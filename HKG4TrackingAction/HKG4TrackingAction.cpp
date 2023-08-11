#include "HKG4TrackingAction.h"

#include "G4OpticalPhoton.hh"

#include "WCSimTrajectory.hh"
#include "WCSimTrackInformation.hh"

using namespace HK::GHOST::G4;

HKG4TrackingAction::HKG4TrackingAction() : Tool(), G4UserTrackingAction() {

	std::cerr << "Default tracking processes/particles should be nothing. Move all to the config file" << std::endl;
	
	ProcessList.insert("Decay");  // Michel e- from pi+ and mu+
	ProcessList.insert("conv");   // Products of gamma conversion

	// Includes Muon decay from K-shell: for Michel
	// e- from mu0. This dominates/replaces the mu- decay
	//Turned off to match WCSim
	//ProcessList.insert("muMinusCaptureAtRest") ;
	
	ProcessList.insert("nCapture");

	// Can check here if the photon comes from WLS
	ProcessList.insert("OpWLS");

	ParticleList.insert(111);  // pi0
	ParticleList.insert(211);  // pion+
	ParticleList.insert(-211);
	ParticleList.insert(321);
	ParticleList.insert(-321);  // kaon-
	ParticleList.insert(311);   // kaon0
	ParticleList.insert(-311);  // kaon0 bar
	ParticleList.insert(11);   // e-
	ParticleList.insert(-11);  // e+
	ParticleList.insert(13);   // mu-
	ParticleList.insert(-13);  // mu+

	//Turned off to match WCSim
	//ParticleList.insert(22); // gammas (high energy photons)
	
	// don't put optical photons there or there'll be too many

	// protons and neutrons
	ParticleList.insert(2212);
	ParticleList.insert(2112);

	percentageOfCherenkovPhotonsToDraw = 0.0;

	// Max time for radioactive decay:
	fMaxTime    = 1. * CLHEP::second;
	fTime_birth = 0.;

}

HKG4TrackingAction::~HKG4TrackingAction() {};

bool HKG4TrackingAction::Initialise(std::string configfile, DataModel& data) {

	if(configfile != "")
		m_variables.Initialise(configfile);
	// m_variables.Print();

	m_data = &data;
	m_log  = m_data->Log;

	if(!m_variables.Get("verbose", m_verbose))
		m_verbose = 1;

	m_data->m_p_g4_run_manager->SetUserAction(this);

	//set options
	float fraction_of_optical_photons_to_save;
	if(!m_variables.Get("fraction_of_optical_photons_to_save", fraction_of_optical_photons_to_save)) {
		fraction_of_optical_photons_to_save = 0;
		std::cout << "fraction_of_optical_photons_to_save option not given. Defaulting to 0" << std::endl;
	}
	percentageOfCherenkovPhotonsToDraw = fraction_of_optical_photons_to_save;
	
	int track_particles_by_pid;
	if(m_variables.Get("track_particles_by_pid", track_particles_by_pid)) {
		std::cout << "Tracking all particles with PID = " << track_particles_by_pid << std::endl;
		ProcessList.insert(track_particles_by_pid);
	}

	std::string track_particles_by_process;
	if(m_variables.Get("track_particles_by_process", track_particles_by_process)) {
		std::cout << "Tracking all particles with process = " << track_particles_by_process << std::endl;
		ProcessList.insert(track_particles_by_process);
	}

	std::cerr << "TODO: work out how to read a vector of values (or some other way of getting multiple values)" << std::endl;
	/*
	std::vector<int> track_particles_by_pid;
	m_variables.Get("track_particles_by_pid", track_particles_by_pid);
	for(int pid : track_particles_by_pid) {
		std::cout << "Tracking all particles with PID = " << pid << std::endl;
		ProcessList.insert(pid);
	}

	std::vector<std::string> *track_particles_by_process;
	m_variables.Get("track_particles_by_process", track_particles_by_process);
	for(std::string process : *track_particles_by_process) {
		std::cout << "Tracking all particles with process = " << process << std::endl;
		ProcessList.insert(process);
	}
	*/
	
	return true;
}

bool HKG4TrackingAction::Execute() {

	return true;
}

bool HKG4TrackingAction::Finalise() {

	return true;
}


void HKG4TrackingAction::PreUserTrackingAction(const G4Track* aTrack) {
	// TF: userdefined now
	// G4double percentageOfCherenkovPhotonsToDraw = 100.0;
	//  if larger than zero, will keep trajectories of many secondaries as well
	//  and store them in output file. Difficult to control them all, so best only
	//  use for visualization, not for storing in ROOT.

	if(aTrack->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition() ||
	   G4UniformRand() < percentageOfCherenkovPhotonsToDraw / 100.) {
		WCSimTrajectory* thisTrajectory = new WCSimTrajectory(aTrack);
		fpTrackingManager->SetTrajectory(thisTrajectory);
		fpTrackingManager->SetStoreTrajectory(true);
	}
	else
		fpTrackingManager->SetStoreTrajectory(false);

	// Kill nucleus generated after TrackID 1
	G4ParticleDefinition* particle = aTrack->GetDefinition();
	G4String name                  = particle->GetParticleName();
	G4double fCharge               = particle->GetPDGCharge();

	G4Track* tr = (G4Track*)aTrack;
	if(aTrack->GetTrackID() == 1) {
		// Re-initialize time
		fTime_birth = 0;
		// Ask G4 to kill the track when all secondary are done (will exclude other decays)
		if(fCharge > 2.)
			tr->SetTrackStatus(fStopButAlive);
	}

	if(aTrack->GetTrackID() == 2) {
		// First track of the decay save time
		fTime_birth = aTrack->GetGlobalTime();
	}
}

void HKG4TrackingAction::PostUserTrackingAction(const G4Track* aTrack) {
	// added by M Fechner
	const G4VProcess* creatorProcess = aTrack->GetCreatorProcess();
	// if ( creatorProcess )

	WCSimTrackInformation* anInfo;
	if(aTrack->GetUserInformation())
		anInfo = (WCSimTrackInformation*)(aTrack->GetUserInformation());  // eg. propagated to all secondaries
		                                                                  // blelow.
	else
		anInfo = new WCSimTrackInformation();

	/** TF's particle list (ToDo: discuss/converge)

	 // is it a primary ?
	// is the process in the set ? eg. Michel e-, but only keep e-
	// is the particle in the set ? eg. pi0, pi+-, K, p, n
	// is it a gamma above 50 MeV ? OR gamma from nCapture? ToDo: Oxygen de-excitation
	// is it a muon that can still produce Cherenkov light?
	// due to lazy evaluation of the 'or' in C++ the order is important
	if( aTrack->GetParentID()==0 || // Primary
	    ((creatorProcess!=0) && ProcessList.count(creatorProcess->GetProcessName())
	     && aTrack->GetMomentum().mag() > .5*MeV && abs(aTrack->GetDefinition()->GetPDGEncoding()) == 11) ||
	    (ParticleList.count(aTrack->GetDefinition()->GetPDGEncoding()) )
	    || (aTrack->GetDefinition()->GetPDGEncoding()==22 && aTrack->GetVertexKineticEnergy() > 4.*MeV) //
	Bugfixed: need vertex kinetic energy for gamma, rest is zero. ToDo: check whether pi0 gamma's are saved!
	    || (aTrack->GetDefinition()->GetPDGEncoding()==22 && creatorProcess->GetProcessName() == "nCapture")
	    || (abs(aTrack->GetDefinition()->GetPDGEncoding()== 13) && aTrack->GetMomentum().mag() > 110.0*MeV)
	//mu+- above Cherenkov Threshold in water (119 MeV/c)

	**/
	// TF: Currently use the nuPRISM one

	// is it a primary ?
	// is the process in the set ?
	// is the particle in the set ?
	// is it a gamma above 1 MeV ?
	// is it a mu- capture at rest above 1 MeV ?
	// due to lazy evaluation of the 'or' in C++ the order is important
	if(aTrack->GetParentID() == 0 ||
	   ((creatorProcess != 0) && ProcessList.count(creatorProcess->GetProcessName())) ||
	   (ParticleList.count(aTrack->GetDefinition()->GetPDGEncoding())) ||
	   (aTrack->GetDefinition()->GetPDGEncoding() == 22 && aTrack->GetTotalEnergy() > 1.0 * CLHEP::MeV) ||
	   (creatorProcess->GetProcessName() == "muMinusCaptureAtRest" && aTrack->GetTotalEnergy() > 1.0 * CLHEP::MeV)) {
		// if so the track is worth saving
		anInfo->WillBeSaved(true);

		// G4cout << "track # " << aTrack->GetTrackID() << " is worth saving\n";
		// G4cout << "It is a " <<aTrack->GetDefinition()->GetParticleName() << G4endl;

		// For Ch hits: use Parent ID of actual mother process:
		//  Decay: keep both decaying particle and Michel e-, Hit Parent ID should be Track ID from Michel e-
		//  Pi0 : keep both pi0 and gamma's, Hit Parent ID should be Track ID from gamma
		//  nCapture: keep both n and gamma, Hit Parent ID should be Track ID from gamma.
		//  Hits from LE electrons from muIonization, etc. : Hit Parent ID should be from Mother particle, not
		//  secondary track ID.

		if(aTrack->GetDefinition()->GetPDGEncoding() == 111)
			pi0List.insert(aTrack->GetTrackID());  // list of all pi0-s

		// Be careful with gamma's. I want the ones closest to the actual mother process, not all secondaries.
		if(aTrack->GetDefinition()->GetPDGEncoding() == 22) {
			// also use lazy evaluation of "or" here:
			if(aTrack->GetParentID() ==
			       0 ||  // then this gamma has no creator process (eg. nRooTracker particles)
			   pi0List.count(aTrack->GetParentID()) ||
			   (creatorProcess->GetProcessName() == "nCapture") ||
			   (creatorProcess->GetProcessName() == "NeutronInelastic"))
				anInfo->SetPrimaryParentID(aTrack->GetTrackID());
		}
		// TF: crucial bugfix: I want this for all tracks that I save to match Ch hits with tracks that can
		//  produce Cherenkov light.
		else
			anInfo->SetPrimaryParentID(aTrack->GetTrackID());
	}
	else {
		anInfo->WillBeSaved(false);
	}

	G4Track* theTrack = (G4Track*)aTrack;
	theTrack->SetUserInformation(anInfo);

	// pass primary parent ID to children
	G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
	if(secondaries) {
		size_t nSeco = secondaries->size();
		if(nSeco > 0) {
			for(size_t i = 0; i < nSeco; i++) {
				WCSimTrackInformation* infoSec = new WCSimTrackInformation(anInfo);
				if(anInfo->isSaved()) {  // Parent is primary, so we want start pos & time of this secondary
					infoSec->SetPhotonStartTime((*secondaries)[i]->GetGlobalTime());
					infoSec->SetPhotonStartPos((*secondaries)[i]->GetPosition());
					infoSec->SetPhotonStartDir((*secondaries)[i]->GetMomentumDirection());
				}
				infoSec->WillBeSaved(false);  // ADDED BY MFECHNER, temporary, 30/8/06
				(*secondaries)[i]->SetUserInformation(infoSec);
			}
		}
	}

	if(aTrack->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition())
	// if (aTrack->GetDefinition()->GetPDGCharge() == 0)
	{
		WCSimTrajectory* currentTrajectory = (WCSimTrajectory*)fpTrackingManager->GimmeTrajectory();

		G4ThreeVector currentPosition    = aTrack->GetPosition();
		G4VPhysicalVolume* currentVolume = aTrack->GetVolume();

		currentTrajectory->SetStoppingPoint(currentPosition);
		currentTrajectory->SetStoppingVolume(currentVolume);

		if(anInfo->isSaved())
			currentTrajectory->SetSaveFlag(true);  // mark it for WCSimEventAction ;
		else
			currentTrajectory->SetSaveFlag(false);  // mark it for WCSimEventAction ;
	}
}

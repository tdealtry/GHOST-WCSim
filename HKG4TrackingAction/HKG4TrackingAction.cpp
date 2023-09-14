#include "HKG4TrackingAction.h"

using namespace HK::GHOST::G4;

HKG4TrackingAction::HKG4TrackingAction() : Tool() {}

bool HKG4TrackingAction::Initialise(std::string configfile, DataModel& data) {

	if(configfile != "")
		m_variables.Initialise(configfile);
	// m_variables.Print();

	m_data = &data;
	m_log  = m_data->Log;

	if(!m_variables.Get("verbose", m_verbose))
		m_verbose = 1;

	m_p_wcsim_tracking_action = new WCSimTrackingAction();

	G4RunManager::GetRunManager()->SetUserAction(m_p_wcsim_tracking_action);

	// set options
	float fraction_of_optical_photons_to_save;
	if(!m_variables.Get("fraction_of_optical_photons_to_save", fraction_of_optical_photons_to_save)) {
		fraction_of_optical_photons_to_save = 0;
		std::cout << "fraction_of_optical_photons_to_save option not given. Defaulting to 0" << std::endl;
	}
	m_p_wcsim_tracking_action->SetFractionChPhotons(fraction_of_optical_photons_to_save);

	int track_particles_by_pid;
	if(m_variables.Get("track_particles_by_pid", track_particles_by_pid)) {
		std::cout << "Tracking all particles with PID = " << track_particles_by_pid << std::endl;
		m_p_wcsim_tracking_action->AddParticle(track_particles_by_pid);
	}

	std::string track_particles_by_process;
	if(m_variables.Get("track_particles_by_process", track_particles_by_process)) {
		std::cout << "Tracking all particles with process = " << track_particles_by_process << std::endl;
		m_p_wcsim_tracking_action->AddProcess(track_particles_by_process);
	}

	std::cerr
	    << "TODO: work out how to read a vector of values (or some other way of getting multiple values) in order to setup which particles/processes should be tracked"
	    << std::endl;
	/*
	std::vector<int> track_particles_by_pid;
	m_variables.Get("track_particles_by_pid", track_particles_by_pid);
	for(int pid : track_particles_by_pid) {
	    std::cout << "Tracking all particles with PID = " << pid << std::endl;
	    m_p_wcsim_tracking_action->AddParticle(pid);
	}

	std::vector<std::string> *track_particles_by_process;
	m_variables.Get("track_particles_by_process", track_particles_by_process);
	for(std::string process : *track_particles_by_process) {
	    std::cout << "Tracking all particles with process = " << process << std::endl;
	    m_p_wcsim_tracking_action->AddProcess(process);
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

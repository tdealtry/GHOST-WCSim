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

	m_p_wcsim_tracking_action = std::unique_ptr<WCSimTrackingAction>(new WCSimTrackingAction);

	m_data->m_p_g4_run_manager->SetUserAction(m_p_wcsim_tracking_action.get());

	//set options
	float fraction_of_optical_photons_to_save;
	if(!m_variables.Get("fraction_of_optical_photons_to_save", fraction_of_optical_photons_to_save)) {
		fraction_of_optical_photons_to_save = 0;
		std::cout << "fraction_of_optical_photons_to_save option not given. Defaulting to 0" << std::endl;
	}
	m_p_wcsim_tracking_action.get()->SetFractionChPhotons(fraction_of_optical_photons_to_save);
		
	return true;
}

bool HKG4TrackingAction::Execute() {

	return true;
}

bool HKG4TrackingAction::Finalise() {

	return true;
}

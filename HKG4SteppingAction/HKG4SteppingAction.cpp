#include "HKG4SteppingAction.h"

using namespace HK::GHOST::G4;

HKG4SteppingAction::HKG4SteppingAction() : Tool() {}

bool HKG4SteppingAction::Initialise(std::string configfile, DataModel& data) {

	if(configfile != "")
		m_variables.Initialise(configfile);
	// m_variables.Print();

	m_data = &data;
	m_log  = m_data->Log;

	if(!m_variables.Get("verbose", m_verbose))
		m_verbose = 1;

	m_data->m_p_wcsim_stepping_action = std::unique_ptr<WCSimSteppingAction>(new WCSimSteppingAction(m_data->m_p_wcsim_run_action.get(), m_data->m_p_wcsim_detector_construction.get()));

	m_data->m_p_g4_run_manager->SetUserAction(m_data->m_p_wcsim_stepping_action.get());

	// set options
	std::cerr << "TODO move the options from the mac file into the toolchain config" << std::endl;
	
	return true;
}

bool HKG4SteppingAction::Execute() {

	return true;
}

bool HKG4SteppingAction::Finalise() {

	return true;
}

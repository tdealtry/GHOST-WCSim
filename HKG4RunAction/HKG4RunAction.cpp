#include "HKG4RunAction.h"

using namespace HK::GHOST::G4;

HKG4RunAction::HKG4RunAction() : Tool() {}

bool HKG4RunAction::Initialise(std::string configfile, DataModel& data) {

	if(configfile != "")
		m_variables.Initialise(configfile);
	// m_variables.Print();

	m_data = &data;
	m_log  = m_data->Log;

	if(!m_variables.Get("verbose", m_verbose))
		m_verbose = 1;

	m_data->m_p_wcsim_run_action =
	    std::unique_ptr<WCSimRunAction>(new WCSimRunAction(m_data->m_p_wcsim_detector_construction.get(),
	                                                       m_data->m_p_wcsim_random_parameters.get()));

	G4RunManager::GetRunManager()->SetUserAction(m_data->m_p_wcsim_run_action.get());

	// set options
	std::cerr << "TODO move the options from the mac file into the toolchain config" << std::endl;

	return true;
}

bool HKG4RunAction::Execute() {

	return true;
}

bool HKG4RunAction::Finalise() {

	return true;
}

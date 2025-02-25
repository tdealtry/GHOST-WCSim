#include "HKG4EventAction.h"

using namespace HK::GHOST::G4;

HKG4EventAction::HKG4EventAction() : Tool() {}

bool HKG4EventAction::Initialise(std::string configfile, DataModel& data) {

	if(configfile != "")
		m_variables.Initialise(configfile);
	// m_variables.Print();

	m_data = &data;
	m_log  = m_data->Log;

	if(!m_variables.Get("verbose", m_verbose))
		m_verbose = 1;

	m_data->m_p_wcsim_event_action = std::unique_ptr<WCSimEventAction>(
	    new WCSimEventAction(m_data->m_p_wcsim_run_action.get(),
	                         m_data->m_p_wcsim_detector_construction.get(),
	                         m_data->m_p_wcsim_primary_generator_action.get()));

	G4RunManager::GetRunManager()->SetUserAction(m_data->m_p_wcsim_event_action.get());

	// set options
	std::cerr << "TODO move the options from the mac file into the toolchain config" << std::endl;

	return true;
}

bool HKG4EventAction::Execute() {

	return true;
}

bool HKG4EventAction::Finalise() {

	return true;
}

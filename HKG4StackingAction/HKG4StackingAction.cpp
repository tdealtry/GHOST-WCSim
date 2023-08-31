#include "HKG4StackingAction.h"

using namespace HK::GHOST::G4;

HKG4StackingAction::HKG4StackingAction() : Tool() {}

bool HKG4StackingAction::Initialise(std::string configfile, DataModel& data) {

	if(configfile != "")
		m_variables.Initialise(configfile);
	// m_variables.Print();

	m_data = &data;
	m_log  = m_data->Log;

	if(!m_variables.Get("verbose", m_verbose))
		m_verbose = 1;

	m_data->m_p_wcsim_stacking_action = std::unique_ptr<WCSimStackingAction>(
	    new WCSimStackingAction(m_data->m_p_wcsim_detector_construction.get()));

	m_data->m_p_g4_run_manager->SetUserAction(m_data->m_p_wcsim_stacking_action.get());

	// set options
	std::cerr << "TODO move the options from the mac file into the toolchain config" << std::endl;

	return true;
}

bool HKG4StackingAction::Execute() {

	return true;
}

bool HKG4StackingAction::Finalise() {

	return true;
}

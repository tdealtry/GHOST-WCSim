#include "HKG4TrackingAction.h"

#include "WCSimTrackingAction.hh"

using namespace HK::Ghost::G4;

HKG4TrackingAction::HKG4TrackingAction() : Tool() {}

bool HKG4TrackingAction::Initialise(std::string configfile, DataModel& data) {

	if(configfile != "")
		m_variables.Initialise(configfile);
	// m_variables.Print();

	m_data = &data;
	m_log  = m_data->Log;

	if(!m_variables.Get("verbose", m_verbose))
		m_verbose = 1;

	m_p_UI = G4UImanager::GetUIpointer();

	m_data->m_p_run_manager->SetUserAction(new WCSimTrackingAction());

	return true;
}

bool HKG4TrackingAction::Execute() {

	return true;
}

bool HKG4TrackingAction::Finalise() {

	return true;
}

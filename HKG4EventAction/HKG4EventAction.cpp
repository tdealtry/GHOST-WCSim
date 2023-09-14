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

	G4RunManager::GetRunManager()->SetUserAction(
	    new WCSimEventAction(dynamic_cast<WCSimRunAction*>(G4RunManager::GetRunManager()->GetUserRunAction()),
	                         dynamic_cast<WCSimDetectorConstruction*>(
	                             G4RunManager::GetRunManager()->GetUserDetectorConstruction()),
	                         dynamic_cast<WCSimPrimaryGeneratorAction*>(
	                             G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction())));

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

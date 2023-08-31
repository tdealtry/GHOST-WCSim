#include "HKG4PrimaryGeneratorAction.h"

using namespace HK::GHOST::G4;

HKG4PrimaryGeneratorAction::HKG4PrimaryGeneratorAction() : Tool() {}

bool HKG4PrimaryGeneratorAction::Initialise(std::string configfile, DataModel& data) {

	if(configfile != "")
		m_variables.Initialise(configfile);
	// m_variables.Print();

	m_data = &data;
	m_log  = m_data->Log;

	if(!m_variables.Get("verbose", m_verbose))
		m_verbose = 1;

	if(!m_data->m_p_wcsim_detector_construction.get()) {
		std::cerr
		    << "Pointer to WCSimDetectorConstruction not found. Ensure HKG4DetectorConstruction comes before HKG4PrimaryGeneratorAction in the toolchain. Exiting"
		    << std::endl;
		return false;
	}

	m_data->m_p_wcsim_primary_generator_action = std::unique_ptr<WCSimPrimaryGeneratorAction>(
	    new WCSimPrimaryGeneratorAction(m_data->m_p_wcsim_detector_construction.get()));

	m_data->m_p_g4_run_manager->SetUserAction(m_data->m_p_wcsim_primary_generator_action.get());

	// set options
	std::cerr << "TODO move the options from the mac file into the toolchain config" << std::endl;

	return true;
}

bool HKG4PrimaryGeneratorAction::Execute() {

	return true;
}

bool HKG4PrimaryGeneratorAction::Finalise() {

	return true;
}

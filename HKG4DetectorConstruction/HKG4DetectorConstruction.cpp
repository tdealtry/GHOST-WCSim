#include "HKG4DetectorConstruction.h"

using namespace HK::GHOST::G4;

HKG4DetectorConstruction::HKG4DetectorConstruction() : Tool() {}

bool HKG4DetectorConstruction::Initialise(std::string configfile, DataModel& data) {

	if(configfile != "")
		m_variables.Initialise(configfile);
	// m_variables.Print();

	m_data = &data;
	m_log  = m_data->Log;

	if(!m_variables.Get("verbose", m_verbose))
		m_verbose = 1;

	enum DetConfiguration { wfm = 1, fwm = 2 };

	G4int WCSimConfiguration = fwm;

	m_data->m_p_wcsim_detector_construction = std::unique_ptr<WCSimDetectorConstruction>(
	    new WCSimDetectorConstruction(WCSimConfiguration, m_data->m_p_g4_tuning_pars.get()));

	G4RunManager::GetRunManager()->SetUserInitialization(m_data->m_p_wcsim_detector_construction.get());

	// set options
	std::cerr << "TODO move the options from the mac file into the toolchain config" << std::endl;

	return true;
}

bool HKG4DetectorConstruction::Execute() {

	return true;
}

bool HKG4DetectorConstruction::Finalise() {

	return true;
}

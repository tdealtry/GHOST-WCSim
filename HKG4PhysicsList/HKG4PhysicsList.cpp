#include "HKG4PhysicsList.h"

#include "../src/Utilities.h"

#include "WCSimRunAction.hh"

using namespace HK::Ghost::G4;

HKG4PhysicsList::HKG4PhysicsList() : Tool() {}

bool HKG4PhysicsList::Initialise(std::string configfile, DataModel& data) {

	if(configfile != "")
		m_variables.Initialise(configfile);
	// m_variables.Print();

	m_data = &data;
	m_log  = m_data->Log;

	if(!m_variables.Get("verbose", m_verbose))
		m_verbose = 1;

	//m_p_UI = G4UImanager::GetUIpointer();

	std::string wcsim_dir = utils::GetEnvironmentVariableWithDefault("WCSIM_BUILD_DIR", "./");
	std::string wcsim_mac_job_opt_filename =
		HK::Ghost::utils::GetConfigFilename(m_variables, "wcsim_mac_job_opt_filename", (wcsim_dir + "/macros/jobOptions.mac").c_str());

	// Set up the messenger hooks here, initialize the actual list after loading jobOptions.mac
	WCSimPhysicsListFactory* m_p_phys_factory = new WCSimPhysicsListFactory();

	m_p_UI = G4UImanager::GetUIpointer();
	m_p_UI->ApplyCommand("/control/execute " + wcsim_mac_job_opt_filename);

	// Initialize the physics factory to register the selected physics.
	m_p_phys_factory->InitializeList();
	m_data->m_p_run_manager->SetUserInitialization(m_p_phys_factory);
																				 
	return true;
}

bool HKG4PhysicsList::Execute() {

	return true;
}

bool HKG4PhysicsList::Finalise() {

	m_p_phys_factory->SaveOptionsToOutput(static_cast<const WCSimRunAction*>(m_data->m_p_run_manager->GetUserRunAction())->GetRootOptions());
	
	return true;
}

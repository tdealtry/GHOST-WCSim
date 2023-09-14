#include "HKG4PhysicsListFactory.h"

using namespace HK::GHOST::G4;

HKG4PhysicsListFactory::HKG4PhysicsListFactory() : Tool() {}

bool HKG4PhysicsListFactory::Initialise(std::string configfile, DataModel& data) {

	if(configfile != "")
		m_variables.Initialise(configfile);
	// m_variables.Print();

	m_data = &data;
	m_log  = m_data->Log;

	if(!m_variables.Get("verbose", m_verbose))
		m_verbose = 1;

	// Initialize the physics factory to register the selected physics.

	// First set up the messenger hooks
	m_data->m_p_wcsim_physics_list_factory = new WCSimPhysicsListFactory;

	// then read the mac file
	m_data->m_p_UI->ApplyCommand("/control/execute " + m_data->m_wcsim_mac_job_opt_filename);
	std::cerr << "TODO move the options from the mac file into the toolchain config" << std::endl;

	// Now we can finally setup the physics list based on the options
	m_data->m_p_wcsim_physics_list_factory->InitializeList();

	G4RunManager::GetRunManager()->SetUserInitialization(m_data->m_p_wcsim_physics_list_factory);

	return true;
}

bool HKG4PhysicsListFactory::Execute() {

	return true;
}

bool HKG4PhysicsListFactory::Finalise() {

	return true;
}

#include "WCSim_exe.h"

#include "WCSimRandomParameters.hh"
#include "WCSimTuningParameters.hh"

using namespace HK::GHOST::G4;

bool HK::GHOST::utils::FileExists(const char* filename) {
	bool exists = access(filename, F_OK) != -1;
	if(!exists) {
		std::cerr << filename << " not found or inaccessible." << std::endl;
	}
	return exists;
}

std::string HK::GHOST::utils::GetEnvironmentVariableWithDefault(const char* variable,
                                                                const char* default_value) {
	std::string env;
	char* env_temp = std::getenv(variable);
	if(env_temp == NULL || !env_temp[0]) {  // make sure it's non-empty
		std::cout << "Environment variable $" << variable << " not set." << std::endl
		          << " Using default value of " << default_value << std::endl;
		env = default_value;
	}
	else {
		env = env_temp;
		std::cout << "Found environment variable $" << variable << " = " << env << std::endl;
	}
	return env;
}

std::string WCSim_exe::GetConfigFilename(const char* config_param, const char* default_filename) {

	std::string filename;
	if(!m_variables.Get(config_param, filename))
		filename = default_filename;
	if(!utils::FileExists(filename.c_str())) {
		std::cerr << "File: " << filename << " not found." << std::endl
		          << "Please give a valid option in the config file to: " << config_param << std::endl
		          << "Exiting..." << std::endl;
		exit(-1);
	}
	std::cout << "Using filename: " << filename << " for parameter: " << config_param << std::endl;
	return filename;
}

WCSim_exe::WCSim_exe() : Tool() {}

bool WCSim_exe::Initialise(std::string configfile, DataModel& data) {

	if(configfile != "")
		m_variables.Initialise(configfile);
	// m_variables.Print();

	m_data = &data;
	m_log  = m_data->Log;

	///////////
	// GET ALL THE PARAMETERS FROM THE CONFIG FILE
	///////////
	if(!m_variables.Get("verbose", m_verbose))
		m_verbose = 1;

	// get all the mac files
	std::string wcsim_dir = utils::GetEnvironmentVariableWithDefault("WCSIMDIR", "./");
	m_data->m_wcsim_mac_job_opt_filename =
	    GetConfigFilename("wcsim_mac_job_opt_filename", (wcsim_dir + "/macros/jobOptions.mac").c_str());
	std::string wcsim_mac_tuning_filename =
	    GetConfigFilename("wcsim_mac_tuning_filename", (wcsim_dir + "/macros/tuning_parameters.mac").c_str());
	m_wcsim_mac_filename = GetConfigFilename("wcsim_mac_filename", (wcsim_dir + "/WCSim.mac").c_str());

	// get the number of events
	if(!m_variables.Get("number_of_events", m_number_of_events)) {
		std::cout << "number_of_events not found in config file. Just simulating 1 event" << std::endl;
		m_number_of_events = 1;
	}
	// initialise the current event number
	m_data->m_current_event = 0;

	///////////
	// SETUP GEANT4
	///////////

	// Construct the default run manager
	new G4RunManager;

	// get the pointer to the UI manager
	m_data->m_p_UI = G4UImanager::GetUIpointer();

	// Set up the tuning parameters that need to be read before the detector
	//  construction is done
	m_data->m_p_g4_tuning_pars = unique_ptr<WCSimTuningParameters>(new WCSimTuningParameters);
	std::cout << "Processing tuning parameter file " << wcsim_mac_tuning_filename << std::endl;
	m_data->m_p_UI->ApplyCommand("/control/execute " + wcsim_mac_tuning_filename);

	// define random number generator parameters
	m_data->m_p_wcsim_random_parameters = unique_ptr<WCSimRandomParameters>(new WCSimRandomParameters);

	// UserInitialization classes (mandatory)

	// Set user action classes

	return true;
}

bool WCSim_exe::Execute() {
	if(!m_data->m_p_wcsim_detector_construction.get()) {
		std::cerr << "Pointer to WCSimDetectorConstruction not found. Exiting" << std::endl;
		return false;
	}
	if(!m_data->m_p_wcsim_primary_generator_action.get()) {
		std::cerr << "Pointer to WCSimPrimaryGeneratorAction not found. Exiting" << std::endl;
		return false;
	}

	if(m_data->m_current_event == 0) {
		// save all the options from WCSimTuningParameters and WCSimPhysicsListFactory
		//(set in e.g. tuning_parameters.mac and jobOptions.mac)
		m_data->m_p_g4_tuning_pars->SaveOptionsToOutput(m_data->m_p_wcsim_run_action->GetRootOptions());
		m_data->m_p_wcsim_physics_list_factory->SaveOptionsToOutput(
		    m_data->m_p_wcsim_run_action->GetRootOptions());

		// Initialize G4 kernel
		G4RunManager::GetRunManager()->Initialize();

		m_data->m_p_UI->ApplyCommand("/control/execute " + m_wcsim_mac_filename);
	}

	if(m_data->m_current_event < m_number_of_events) {
		m_data->m_p_UI->ApplyCommand("/run/beamOn 1");
	}
	m_data->m_current_event++;
	if(m_data->m_current_event >= m_number_of_events)
		m_data->vars.Set("StopLoop", 1);
	return true;
}

bool WCSim_exe::Finalise() {

	return true;
}

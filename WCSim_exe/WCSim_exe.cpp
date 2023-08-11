#include "WCSim_exe.h"

#include "WCSimEventAction.hh"
#include "WCSimPhysicsListFactory.hh"
#include "WCSimPrimaryGeneratorAction.hh"
#include "WCSimRandomParameters.hh"
#include "WCSimRunAction.hh"
#include "WCSimStackingAction.hh"
#include "WCSimSteppingAction.hh"
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
	std::string wcsim_mac_job_opt_filename =
	    GetConfigFilename("wcsim_mac_job_opt_filename", (wcsim_dir + "/macros/jobOptions.mac").c_str());
	std::string wcsim_mac_tuning_filename =
	    GetConfigFilename("wcsim_mac_tuning_filename", (wcsim_dir + "/macros/tuning_parameters.mac").c_str());
	std::string wcsim_mac_filename =
	    GetConfigFilename("wcsim_mac_filename", (wcsim_dir + "/WCSim.mac").c_str());

	// get the number of events
	if(!m_variables.Get("number_of_events", m_number_of_events)) {
		std::cout << "number_of_events not found in config file. Just simulating 1 event" << std::endl;
		m_number_of_events = 1;
	}
	// initialise the current event number
	m_current_event = 0;

	///////////
	// SETUP GEANT4
	///////////

	// Construct the default run manager
	m_data->m_p_g4_run_manager = unique_ptr<G4RunManager>(new G4RunManager);

	// get the pointer to the UI manager
	m_p_UI = G4UImanager::GetUIpointer();

	// Set up the tuning parameters that need to be read before the detector
	//  construction is done
	m_data->m_p_g4_tuning_pars = unique_ptr<WCSimTuningParameters>(new WCSimTuningParameters);
	std::cout << "Processing tuning parameter file " << wcsim_mac_tuning_filename << std::endl;
	m_p_UI->ApplyCommand("/control/execute " + wcsim_mac_tuning_filename);

	// define random number generator parameters
	WCSimRandomParameters* randomparameters = new WCSimRandomParameters();

	// UserInitialization classes (mandatory)

	// Added selectable physics lists 2010-07 by DMW
	// Set up the messenger hooks here, initialize the actual list after loading jobOptions.mac
	WCSimPhysicsListFactory* physFactory = new WCSimPhysicsListFactory();

	m_p_UI->ApplyCommand("/control/execute " + wcsim_mac_job_opt_filename);

	// Initialize the physics factory to register the selected physics.
	physFactory->InitializeList();
	m_data->m_p_g4_run_manager->SetUserInitialization(physFactory);

	if(!m_data->m_p_wcsim_detector_construction.get())
		return false;

	// Set user action classes
	WCSimPrimaryGeneratorAction* myGeneratorAction = new WCSimPrimaryGeneratorAction(m_data->m_p_wcsim_detector_construction.get());
	m_data->m_p_g4_run_manager->SetUserAction(myGeneratorAction);

	WCSimRunAction* myRunAction = new WCSimRunAction(m_data->m_p_wcsim_detector_construction.get(), randomparameters);

	// save all the options from WCSimTuningParameters & WCSimPhysicsListFactory
	//(set in tuning_parameters.mac & jobOptions*.mac)
	m_data->m_p_g4_tuning_pars->SaveOptionsToOutput(myRunAction->GetRootOptions());
	physFactory->SaveOptionsToOutput(myRunAction->GetRootOptions());

	m_data->m_p_g4_run_manager->SetUserAction(myRunAction);

	m_data->m_p_g4_run_manager->SetUserAction(new WCSimEventAction(myRunAction, m_data->m_p_wcsim_detector_construction.get(), myGeneratorAction));

	m_data->m_p_g4_run_manager->SetUserAction(new WCSimStackingAction(m_data->m_p_wcsim_detector_construction.get()));

	m_data->m_p_g4_run_manager->SetUserAction(new WCSimSteppingAction(myRunAction, m_data->m_p_wcsim_detector_construction.get()));

	// Initialize G4 kernel
	m_data->m_p_g4_run_manager->Initialize();

	m_p_UI->ApplyCommand("/control/execute " + wcsim_mac_filename);
	return true;
}

bool WCSim_exe::Execute() {
	if(m_current_event < m_number_of_events) {
		m_p_UI->ApplyCommand("/run/beamOn 1");
	}
	m_current_event++;
	if(m_current_event >= m_number_of_events)
		m_data->vars.Set("StopLoop", 1);
	return true;
}

bool WCSim_exe::Finalise() {

	return true;
}

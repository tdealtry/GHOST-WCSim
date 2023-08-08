#include "WCSim_exe.h"

using namespace HK::GHOST;

bool HK::GHOST::utils::FileExists(const char * filename) {
	bool exists = access(filename, F_OK) != -1;
	if(!exists) {
		std::cerr << filename << " not found or inaccessible." << std::endl;
	}
	return exists;
}

std::string HK::GHOST::utils::GetEnvironmentVariableWithDefault(const char * variable,
																																const char * default_value) {
	std::string env;
  char * env_temp = std::getenv(variable);
  if(env_temp == NULL || !env_temp[0]) { // make sure it's non-empty
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

std::string WCSim_exe::GetConfigFilename(const char * config_param,
																				 const char * default_filename) {
	
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

	if(!m_variables.Get("verbose", m_verbose))
		m_verbose = 1;

	//get all the mac files
	std::string wcsim_dir = utils::GetEnvironmentVariableWithDefault("WCSIMDIR", "./");
	std::string wcsim_mac_job_opt_filename = GetConfigFilename("wcsim_mac_job_opt_filename", (wcsim_dir + "/macros/jobOptions.mac").c_str());
	std::string wcsim_mac_tuning_filename = GetConfigFilename("wcsim_mac_tuning_filename", (wcsim_dir + "/macros/tuning_parameters.mac").c_str());
	std::string wcsim_mac_filename = GetConfigFilename("wcsim_mac_filename", (wcsim_dir + "/WCSim.mac").c_str());

	
	return true;
}

bool WCSim_exe::Execute() {

	return true;
}

bool WCSim_exe::Finalise() {

	return true;
}

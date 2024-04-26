#ifndef WCSim_exe_H
#define WCSim_exe_H

#include <iostream>
#include <string>

#include <DataModel.h>
#include "Tool.h"

//#include "G4RunManager.hh"
#include "G4UImanager.hh"

/**
 * \class WCSim_exe
 *
 * Tool to take the role of WCSim.cc
 *
 * $Author: T.Dealtry $
 * $Date: 2023/08/08 14:31:00 $
 */

namespace HK {
	namespace Ghost {
		namespace G4 {
		class WCSim_exe : public Tool {

			public:

				WCSim_exe();  ///< Simple constructor
				bool Initialise(
				    std::string configfile,
				    DataModel& data);  ///< Initialise Function for setting up Tool resources. @param
				                       ///< configfile The path and name of the dynamic configuration file
				                       ///< to read in. @param data A reference to the transient data
				                       ///< class used to pass information between Tools.
				bool Execute();        ///< Execute function used to perform Tool purpose.
				bool Finalise();       ///< Finalise funciton used to clean up resources.

			private:

				//! Get a filename from the config file
				/*!
				    Also checks that the file exists. There is a hard exit if not
				    \param config_param The name of the config file variable
				    \param default_filename A fallback
				    \return The filename
				*/
				std::string GetConfigFilename(const char* config_param, const char* default_filename);

			//G4RunManager* m_p_run_manager;
				G4UImanager* m_p_UI;
			bool m_initialised;
			std::string m_wcsim_mac_filename;
				long m_number_of_events;
				long m_current_event;
		};
		} //namespace G4
		
		namespace utils {
			//! Get an environment variable, falling back to a default value
			/*!
			    \param variable The name of the environment variable
			    \param default_value The default value, to use if the environment variable isn't set or is
			   empty \return A string containing the environment variable if it is set and is not empty, else
			   the default
			*/
			std::string GetEnvironmentVariableWithDefault(const char* variable, const char* default_value);

			//! Check if a file exists and is accessible
			/*!
			    \param filename The name of the file
			    \return True if it exists and is accessible, false if not
			*/
			bool FileExists(const char* filename);

		} // namespace utils
	} // namespace Ghost
} // namespace HK
#endif

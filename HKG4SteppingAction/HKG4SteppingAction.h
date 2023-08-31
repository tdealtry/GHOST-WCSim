#ifndef HKG4SteppingAction_H
#define HKG4SteppingAction_H

#include <iostream>
#include <string>

#include "DataModel.h"
#include "Tool.h"

/**
 * \class HKG4SteppingAction
 *
 * This is a blank template for a Tool used by the script to generate a new custom tool. Please fill out the
 * description and author information.
 *
 * $Author: B.Richards $
 * $Date: 2019/05/28 10:44:00 $
 */

namespace HK {
	namespace GHOST {
		namespace G4 {
			class HKG4SteppingAction : public Tool {

				public:

					HKG4SteppingAction();  ///< Simple constructor
					bool Initialise(
					    std::string configfile,
					    DataModel& data);  ///< Initialise Function for setting up Tool resources. @param
					                       ///< configfile The path and name of the dynamic configuration file
					                       ///< to read in. @param data A reference to the transient data
					                       ///< class used to pass information between Tools.
					bool Execute();        ///< Execute function used to perform Tool purpose.
					bool Finalise();       ///< Finalise funciton used to clean up resources.

				private:
			};

		}  // namespace G4
	}      // namespace GHOST
}  // namespace HK
#endif

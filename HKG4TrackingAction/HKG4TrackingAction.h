#ifndef HKG4TrackingAction_H
#define HKG4TrackingAction_H

#include <iostream>
#include <string>
#include <set>

#include "DataModel.h"
#include "Tool.h"

#include "G4UserTrackingAction.hh"

/**
 * \class HKG4TrackingAction
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
			class HKG4TrackingAction : public Tool, public G4UserTrackingAction {

			public:

				HKG4TrackingAction();  ///< Simple constructor
				~HKG4TrackingAction(); ///< Simple destructor
				
				bool Initialise(std::string configfile,
												DataModel& data);  ///< Initialise Function for setting up Tool resources. @param
				                                   ///< configfile The path and name of the dynamic configuration file
				                                   ///< to read in. @param data A reference to the transient data
				                                   ///< class used to pass information between Tools.
				bool Execute();                    ///< Execute function used to perform Tool purpose.
				bool Finalise();                   ///< Finalise funciton used to clean up resources.

				void PreUserTrackingAction(const G4Track* aTrack);
				void PostUserTrackingAction(const G4Track*);

	private:
				void SetFractionChPhotons(G4double fraction) { percentageOfCherenkovPhotonsToDraw = fraction; }

				void AddProcess(const G4String& process) { ProcessList.insert(process); }

				void AddParticle(G4int pid) { ParticleList.insert(pid); }

				//std::unique_ptr<WCSimTrackingAction> m_p_wcsim_tracking_action;

				std::set<G4String> ProcessList;
				std::set<G4int> ParticleList;
				std::set<G4int> pi0List;

				G4double fTime_birth;
				G4double fMaxTime;

				G4double percentageOfCherenkovPhotonsToDraw;
};

		} // namespace G4
	} // namespace GHOST
} // namespace HK
#endif

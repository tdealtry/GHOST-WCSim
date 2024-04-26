#ifndef HKG4TrackingAction_H
#define HKG4TrackingAction_H

#include <iostream>
#include <string>

#include <DataModel.h>
#include "Tool.h"

#include "G4UImanager.hh"
#include "G4UserTrackingAction.hh"

#include "WCSimTrackingAction.hh"

/**
 * \class HKG4TrackingAction
 *
 * This is a blank template for a Tool used by the script to generate a new custom tool. Please fill out the
 * description and author information.
 *
 */

namespace HK {
	namespace Ghost {
		namespace G4 {
			class HKG4TrackingAction : public Tool, public G4UserTrackingAction {

			public:

				HKG4TrackingAction();  ///< Simple constructor
				bool Initialise(std::string configfile,
												DataModel& data) override;  ///< Initialise Function for setting up Tool resources. @param
				///< configfile The path and name of the dynamic configuration file
				///< to read in. @param data A reference to the transient data
				///< class used to pass information between Tools.
				bool Execute() override;                    ///< Execute function used to perform Tool purpose.
				bool Finalise() override;                   ///< Finalise funciton used to clean up resources.

			private:
				G4UImanager* m_p_UI;

			public:
				void PreUserTrackingAction (const G4Track* aTrack) override;
				void PostUserTrackingAction(const G4Track*) override;

				void SetFractionChPhotons(G4double fraction){percentageOfCherenkovPhotonsToDraw = fraction;}
  
				void AddProcess(const G4String &process){ProcessList.insert(process);}
				void AddParticle(G4int pid){ParticleList.insert(pid);}
				void SetSaveHitProducingTracks(G4bool save){saveHitProducingTracks = save;}
				G4bool GetSaveHitProducingTracks() const {return saveHitProducingTracks;}

			private:
				std::set<G4String> ProcessList;
				std::set<G4int> ParticleList;
				G4bool saveHitProducingTracks = true;
  
				G4double fTime_birth;
				G4double fMaxTime;
				// TF: define in macro now
				G4double percentageOfCherenkovPhotonsToDraw;

				bool SAVE_PHOTON_HISTORY;

				WCSimTrackingMessenger* messenger;
  
				G4int primaryID;
				
			};
		} // namespace G4
	} // namespace Ghost
} // namespace HK
#endif

#include "WCSimTrackingMessenger.hh"
#include "WCSimTrackingAction.hh"

#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"

WCSimTrackingMessenger::WCSimTrackingMessenger(WCSimTrackingAction* trackAction) : myTracking(trackAction) {

	WCSimDir = new G4UIdirectory("/Tracking/");
	WCSimDir->SetGuidance("Commands to change particles to draw and save during tracking");

	particleToTrack = new G4UIcmdWithAnInteger("/Tracking/trackParticle", this);
	particleToTrack->SetGuidance("Command to track all particles of given type");
	particleToTrack->SetParameterName("particleToTrack", false);

	processToTrack = new G4UIcmdWithAString("/Tracking/trackProcess", this);
	processToTrack->SetGuidance("Command to track all particles created by given process");
	processToTrack->SetParameterName("processToTrack", false);
}

WCSimTrackingMessenger::~WCSimTrackingMessenger() {

	delete WCSimDir;
}

void WCSimTrackingMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
if(command == particleToTrack) {
		G4int pid = particleToTrack->GetNewIntValue(newValue);
		myTracking->AddParticle(pid);
		G4cout << "Tracking all particles with PID = " << pid << G4endl;
	}
	else if(command == processToTrack) {
		myTracking->AddProcess(newValue);
		G4cout << "Tracking all particles created by the " << newValue << " process" << G4endl;
	}
}

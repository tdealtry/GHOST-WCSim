#ifndef WCSimWCAddDarkNoise_h
#define WCSimWCAddDarkNoise_h 1

#include <map>
#include <utility>
#include <vector>
#include "G4VDigitizerModule.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "WCSimDarkRateMessenger.hh"
#include "WCSimDetectorConstruction.hh"
#include "WCSimRootOptions.hh"
#include "WCSimWCDigi.hh"
#include "WCSimWCHit.hh"

class WCSimWCAddDarkNoise : public G4VDigitizerModule {
	public:

		WCSimWCAddDarkNoise(G4String name, WCSimDetectorConstruction*, G4String);
		~WCSimWCAddDarkNoise();

	public:

		void AddDarkNoise();
		void AddDarkNoiseBeforeDigi(WCSimWCDigitsCollection* WCHCPMT, double num1, double num2);
		void FindDarkNoiseRanges(WCSimWCDigitsCollection* WCHCPMT, double width);

		// As it inherits from G4VDigitizerModule it needs a digitize class.  Not used
		void Digitize() {}

		void SetDarkRate(double idarkrate) { PMTDarkRate = idarkrate; }

		double GetDarkRate() { return PMTDarkRate; }

		void SetConversion(double iconvrate) { ConvRate = iconvrate; }

		void SetDarkMode(int imode) { DarkMode = imode; }

		int GetDarkMode() { return DarkMode; }

		void SetDarkHigh(double idarkhigh) { DarkHigh = idarkhigh; }

		double GetDarkHigh() { return DarkHigh; }

		void SetDarkLow(double idarklow) { DarkLow = idarklow; }

		double GetDarkLow() { return DarkLow; }

		void SetDarkWindow(int idarkwindow) { DarkWindow = idarkwindow; }

		int GetDarkWindow() { return (int)(DarkWindow); }

		void SaveOptionsToOutput(WCSimRootOptions* wcopt, string tag);

	private:

		void ReInitialize() {
			ranges.clear();
			result.clear();
		}

		void SetPMTDarkDefaults();

		WCSimDarkRateMessenger* DarkRateMessenger;
		double PMTDarkRate;  // kHz
		double ConvRate;     // kHz
		double DarkHigh;     // ns
		double DarkLow;      // ns
		double DarkWindow;   // ns
		int DarkMode;
		bool fCalledAddDarkNoise;

		WCSimDetectorConstruction* myDetector;
		G4String detectorElement;

		std::vector<std::pair<double, double> > ranges;
		std::vector<std::pair<double, double> > result;
};

#endif

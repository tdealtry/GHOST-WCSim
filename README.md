# hk-ToolScripts

## Current installation instructions

From `docker run -it ghcr.io/hyperk/hk-software`
```bash
#Clone hk-ToolApp and associated modules
git clone https://github.com/tdealtry/hk-ToolApp.git
cd hk-ToolApp
git switch ghost
git submodule init
git submodule update

#Setup the generic HK software
source /usr/local/hk/hk-pilot/setup.sh
source /usr/local/hk/hk-software/install-Linux_x86_64-gcc_8-python_3.8.13/setup.sh
source /usr/local/hk/ROOT/install-Linux_x86_64-gcc_8-python_3.8.13/bin/thisroot.sh
source /usr/local/hk/Geant4/install-Linux_x86_64-gcc_8-python_3.8.13/bin/geant4.sh

#Do a make in ToolFrameworkCore (i.e. ignore the `cmake` build)
cd /usr/local/hk/ToolFrameworkCore/src
make

#Clone GHOST-WCSim
cd /usr/local/hk/hk-ToolApp/UserTools/ImportedTools/
git clone https://github.com/tdealtry/GHOST-WCSim.git
cd GHOST-WCSim

#Build the old WCSim code
mkdir OriginalWCSim_build
cd OriginalWCSim_build
cmake ../OriginalWCSim/
make -j4
make install
# and source
source ./this_wcsim.sh

#Hack gray->grey
sed -i 's!gray!grey!' /usr/local/hk/ToolFrameworkCore/src/include/Logging.h
sed -i 's!gray!grey!' /usr/local/hk/ToolFrameworkCore/install-Linux_x86_64-gcc_8-python_3.8.13//include/ToolFrameworkCore/Logging.h

#Finally make GHOST-WCSim
cd /usr/local/hk/hk-ToolApp/UserTools/ImportedTools/GHOST-WCSim/
make

#Tell hk-ToolApp about the tools
# Note I can't see a way to do this in batch. Need to select the tools by hand each time
#  so things are actually commited to my hk-ToolApp branch, so this doesn't have to be done
./Import.sh
## then select WCSim_exe (and other tools starting with HKG4) with the space & press enter to ok
cd /usr/local/hk/hk-ToolApp/UserTools
./ToolSelect.sh
## then select (de)activate, WCSim_exe (and other tools starting with HKG4) with space, then backup or not
#And hack the namespace in Factory.cpp
sed -i 's!new WCSim_exe!new HK::GHOST::G4::WCSim_exe!' Factory/Factory.cpp
sed -i 's!new HKG4!new HK::GHOST::G4::HKG4!' Factory/Factory.cpp

#Finally we can build hk-ToolApp
cd ..
source Setup.sh
make
# soft links so that WCSim is happy
ln -s $WCSIMDIR/data/
ln -s $WCSIMDIR/macros/

#Test
./main ghost
```

## Multiple inheritence of `Tool` & G4 user action classes
One of the ideas of the GHOST design was multiple inheritance, for example
```c++
HKG4TrackingAction::HKG4TrackingAction() : Tool(), G4UserTrackingAction()
```
However Geant4 throws an exception on this, even when the `HKG4TrackingAction` comes after the `WCSim_exe` tool in the toolchain
```
[ERROR]: 
-------- EEEE ------- G4Exception-START -------- EEEE -------

*** ExceptionHandler is not defined ***
*** G4Exception : Tracking0001
      issued by : G4UserTrackingAction::G4UserTrackingAction()
 You are instantiating G4UserTrackingAction BEFORE your
G4VUserPhysicsList is instantiated and assigned to G4RunManager.
 Such an instantiation is prohibited since Geant4 version 8.0. To fix this problem,
please make sure that your main() instantiates G4VUserPhysicsList AND
set it to G4RunManager before instantiating other user action classes
such as G4UserTrackingAction.
*** Fatal Exception ***
-------- EEEE -------- G4Exception-END --------- EEEE -------

[ERROR]: 
[ERROR]: *** G4Exception: Aborting execution ***
```
Why is this happening? It's to do with how the toolchain fundamentally runs. For each step it calls every tool in order
- `Tool::Tool()` i.e. construct the tool
- `Tool::Initialise()`
- `Tool::Execute()` (note it goes round this `n` times)
- `Tool::Finalise()`
- `Tool::~Tool()` i.e. destruct the tool

So basically I'm stuck in an unrunnable state, until I toolify all user action classes...

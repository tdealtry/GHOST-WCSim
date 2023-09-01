# hk-ToolScripts

## Current installation instructions

From `docker run -it ghcr.io/hyperk/hk-software`

If these instructions are out of date, please check `.github/workflows/c-cpp.yml` for something that (should) work
```bash
#Clone hk-ToolApp and associated modules
git clone https://github.com/tdealtry/hk-ToolApp.git
cd hk-ToolApp
git switch ghost
git submodule init
git submodule update

#Setup the generic HK software - should now be done automatically when you load the image
# However there is a bug where `$LD_LIBRARY_PATH` is not set correctly...
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

#Get the submodule (i.e. original WCSim code)
git submodule init
git submodule update

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

#Use my copy of DataModel
# it contains things required by GHOST
# This must be done after building the original WCSim code
cd /usr/local/hk/hk-ToolApp/hk-DataModel
git remote add tom https://github.com/tdealtry/hk-DataModel.git
git fetch tom
git switch --detach tom/main
make clean
make

#Finally make GHOST-WCSim
cd /usr/local/hk/hk-ToolApp/UserTools/ImportedTools/GHOST-WCSim/
make

#Tell hk-ToolApp about the tools
# Note I can't see a way to do this in batch. Need to select the tools by hand each time
#  so things are actually commited to my hk-ToolApp branch, so this doesn't have to be done
./Import.sh
## then select WCSim_exe (and any HKG4 tools) with the space & press enter to ok
cd /usr/local/hk/hk-ToolApp/UserTools
./ToolSelect.sh
## then select (de)activate, WCSim_exe (and any HKG4 tools) with space, then backup or not
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

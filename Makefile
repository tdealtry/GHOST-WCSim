ToolFrameworkPath =  /usr/local/hk/ToolFrameworkCore/src
DataModelPath = /usr/local/hk/hk-ToolApp/hk-DataModel/

Geant4Path = /usr/local/hk/Geant4/install-Linux_x86_64-gcc_8-python_3.8.13/
#WCSimPath = /opt/WCSim/install/
WCSimPath = /opt/hk-ToolApp/UserTools/ImportedTools/GHOST-WCSim/OriginalWCSim_build/
RootPath = /usr/local/hk/ROOT/install-Linux_x86_64-gcc_8-python_3.8.13/

ROOT_INCLUDES  = -I$(shell root-config --incdir)
ROOT_LIBS  = $(shell root-config --libs)
GEANT4_INCLUDES = -I$(Geant4Path)/include/Geant4/
GEANT4_LIBS = $(shell geant4-config --libs)
WCSIM_INCLUDES = -I$(WCSimPath)/include/WCSim/ -DWCSIM_CHECK_GEOMETRY_OVERLAPS=0
WCSIM_LIBS = -L$(WCSimPath)/lib/ -lWCSimCore -lWCSimRoot
EXTERNALS := $(ROOT_INCLUDES) \
	$(ROOT_LIBS) \
	$(GEANT4_INCLUDES) \
	$(GEANT4_LIBS) \
	$(WCSIM_LIBS) \
	$(WCSIM_INCLUDES)

CXXFLAGS = g++ -O3 -fPIC

objects:= $(patsubst %.cpp, %.o, $(filter-out template/%.cpp, $(wildcard */*.cpp)))
libs:= $(subst /,/lib, $(patsubst %.o, %.so, $(objects)))

all: $(libs)

lib%.so: %.o
	$(CXXFLAGS) --shared $*.o -o $@ -I $(DataModelPath) -L $(DataModelPath) -lDataModel -I $(ToolFrameworkPath)/include -L $(ToolFrameworkPath)/lib -lStore -LLogging $(EXTERNALS)

%.o: %.cpp
	$(CXXFLAGS) -c $< -o $@  -I $(DataModelPath) -L $(DataModelPath) -lDataModel -I $(ToolFrameworkPath)/include -L $(ToolFrameworkPath)/lib -lStore -LLogging $(EXTERNALS)

clean:
	rm -f */*.o
	rm -f */*.so

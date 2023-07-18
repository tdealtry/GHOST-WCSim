ToolFrameworkPath =  /usr/local/hk/ToolFrameworkCore/src
DataModelPath = /usr/local/hk/hk-ToolApp/hk-DataModel/

CXXFLAGS = g++ -O3 -fPIC

objects:= $(patsubst %.cpp, %.o, $(filter-out template/%.cpp, $(wildcard */*.cpp)))
libs:= $(subst /,/lib, $(patsubst %.o, %.so, $(objects)))

all: $(libs)

lib%.so: %.o
	$(CXXFLAGS) --shared $*.o -o $@ -I $(DataModelPath) -L $(DataModelPath) -lDataModel -I $(ToolFrameworkPath)/include -L $(ToolFrameworkPath)/lib -lStore -LLogging

%.o: %.cpp
	$(CXXFLAGS) -c $< -o $@  -I $(DataModelPath) -L $(DataModelPath) -lDataModel -I $(ToolFrameworkPath)/include -L $(ToolFrameworkPath)/lib -lStore -LLogging

clean:
	rm -f */*.o
	rm -f */*.so

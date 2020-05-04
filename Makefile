SERVER=server
SUBSCRIBER=subscriber
C_SOURCES=$(wildcard *.c) # select all c source files
CPP_SOURCES=$(wildcard *.cpp) # select all cpp source files
LIBRARY=nope
INCPATHS=include
LIBPATHS=.
LDFLAGS=
CXXFLAGS=-c -Wall -g3
CXX=g++

# Automatic generation of some important lists

# tell the makefile to generate object files for each source file
OBJECTS=$(C_SOURCES:.c=.o) $(CPP_SOURCES:.cpp=.o)
INCFLAGS=$(foreach TMP,$(INCPATHS),-I$(TMP))
LIBFLAGS=$(foreach TMP,$(LIBPATHS),-L$(TMP))

# Set up the output file names for the different output types
BINARY_SERVER=$(SERVER)
BINARY_SUBSCRIBER=$(SUBSCRIBER)

all: $(C_SOURCES) $(CPP_SOURCES) $(BINARY_SERVER) $(BINARY_SUBSCRIBER)

$(BINARY_SERVER): $(filter-out subscriber.o subscriber_utils.o, $(OBJECTS))
	$(CXX) $(LIBFLAGS) $(filter-out subscriber.o subscriber_utils.o, $(OBJECTS)) $(LDFLAGS) -o $@

$(BINARY_SUBSCRIBER): $(filter-out server.o server_utils.o, $(OBJECTS))
	$(CXX) $(LIBFLAGS) $(filter-out server.o server_utils.o, $(OBJECTS)) $(LDFLAGS) -o $@

.cpp.o:
	$(CXX) $(INCFLAGS) $(CXXFLAGS) -fPIC $< -o $@

distclean: clean
	rm -f $(BINARY_SERVER) $(BINARY_SUBSCRIBER)

clean:
	rm -f $(OBJECTS)


CLIENT=client
SOURCES=$(wildcard *.c)
LIBRARY=none
INCPATHS=include
LIBPATHS=
LDFLAGS=
CCFLAGS=-c -Wall -g3
CC=gcc

# Automatic generation of some important lists

# tell the makefile to generate object files for each source file
OBJECTS=$(SOURCES:.c=.o) 
INCFLAGS=$(foreach TMP,$(INCPATHS),-I$(TMP))
LIBFLAGS=$(foreach TMP,$(LIBPATHS),-L$(TMP))

# Set up the output file names for the different output types

all: $(SOURCES) $(CLIENT) 

$(CLIENT): $(OBJECTS)
	$(CC) $(LIBFLAGS) $(OBJECTS) $(LDFLAGS) -o $@

.c.o:
	$(CC) $(INCFLAGS) $(CCFLAGS) -fPIC $< -o $@

distclean: clean
	rm -f $(CLIENT)

clean:
	rm -f $(OBJECTS)



# Makefile to build the DSP parts of ArtyFX

#CC=g++
CFLAGS  += -Wall -fPIC
LDFLAGS += -lsndfile -lpthread
SOURCES=$(wildcard artyfx.cxx dsp/*.cxx dsp/eq/*.cc dsp/rr/*.cxx )
OBJECTS := $(patsubst %.c,%.o, $(SOURCES) )
	OUTPUT=artyfx.so

all: $(SOURCES) $(OUTPUT)
	    
$(OUTPUT): $(OBJECTS) 
	    $(CC) -shared -fPIC -Wl,--no-undefined $(LDFLAGS) $(OBJECTS) -o $@

.cxx.o:
	    $(CC) $(CFLAGS) $< -o $@


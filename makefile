#!/usr/bin/make -f

ifeq ($(shell pkg-config --atleast-version=1.4.6 lv2 || echo no), no)
	$(error "LV2 SDK >= 1.4.6 was not found")
endif

.PHONY: all clean install 

# Includes
INCLUDES=$(shell pkg-config --cflags sndfile cairomm-1.0 ntk ntk_images)

# DSP files
DSP_SOURCES=bitta/dsp/dsp.cxx della/dsp/dsp.cxx ducka/dsp/dsp.cxx filta/dsp/dsp.cxx kuiza/dsp/dsp.cxx kuiza/dsp/eq/filters.cc kuiza/dsp/eq/filters_if.cc kuiza/dsp/eq/exp2ap.cc masha/dsp/masha.cxx panda/dsp/dsp.cxx roomy/dsp/dsp.cxx satma/dsp/dsp.cxx vihda/dsp/dsp.cxx artyfx.cxx
DSP_OBJECTS=bitta/dsp/dsp.o della/dsp/dsp.o ducka/dsp/dsp.o filta/dsp/dsp.o kuiza/dsp/dsp.o kuiza/dsp/eq/filters.cc kuiza/dsp/eq/filters_if.cc kuiza/dsp/eq/exp2ap.cc masha/dsp/masha.o panda/dsp/dsp.o roomy/dsp/dsp.o satma/dsp/dsp.o vihda/dsp/dsp.o artyfx.o
#DSP_OBJECTS = $(DSP_SOURCES:.cxx=.o)

# UI files
UI_SOURCES=bitta/gui/ui.cxx bitta/gui/widget.cxx della/gui/ui.cxx della/gui/widget.cxx  ducka/gui/ducka_ui.cxx ducka/gui/ducka_widget.cxx filta/gui/ui.cxx filta/gui/widget.cxx kuiza/gui/ui.cxx kuiza/gui/widget.cxx masha/gui/masha_widget.cxx masha/gui/masha_ui.cxx panda/gui/ui.cxx panda/gui/widget.cxx roomy/gui/roomy_ui.cxx roomy/gui/roomy_widget.cxx satma/gui/ui.cxx satma/gui/widget.cxx vihda/gui/ui.cxx vihda/gui/widget.cxx artyfx_ui.cxx 
UI_OBJECTS = $(UI_SOURCES:.cxx=.o)

CXX=g++
CXXFLAGS=-g -Wall -fPIC
# Add these for long-winded warnings!
# -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Wredundant-decls -Winline -Wno-long-long  -Wuninitialized -Wconversion

LDFLAGS=-fPIC -shared -Wl,-z,nodelete -Wl,--no-undefined
UI_LDFLAGS=$(LDFLAGS) $(shell pkg-config --libs sndfile cairomm-1.0 ntk ntk_images) 


# compile command:
COMMAND=$(CXX) $(CXXFLAGS)  $(INCLUDES) -c $< -o $@

#%.o: %.cpp %.hxx
# 		$(COMMAND)

%.o: %.cxx
	$(COMMAND)

artyfx_ui.so : $(UI_OBJECTS) artyfx.so
	$(CXX) $(CXXFLAGS) $(UI_OBJECTS)  -o artyfx.lv2/$@  $(UI_LDFLAGS)

artyfx.so : $(DSP_OBJECTS)
	$(CXX) $(CXXFLAGS) $(DSP_OBJECTS) -o artyfx.lv2/$@  $(LDFLAGS)


all: artyfx_ui.so

clean:
	rm artyfx.lv2/artyfx.so
	rm artyfx.lv2/artyfx_ui.so

install:
	cp -r artyfx.lv2/ ~/.lv2/


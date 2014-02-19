
CC=g++
CFLAGS=-O3 -Wall

INCLUDES=$(shell pkg-config --cflags sndfile cairomm-1.0 ntk ntk_images)
LDFLAGS=$(shell pkg-config --libs sndfile cairomm-1.0 ntk ntk_images) -fPIC -shared -Wl,-z,nodelete

UISOURCES=gui/widget.cxx gui/ui.cxx
UIOBJECTS=$(UISOURCES:.cpp=.o)
UI=kuiza.lv2/kuiza_ui.so

DSPSOURCES=dsp/dsp.cxx dsp/eq/exp2ap.cc dsp/eq/filters.cc dsp/eq/filters_if.cc
DSPOBJECTS=$(DSPSOURCES:.cpp=.o)
DSP=kuiza.lv2/kuiza.so

all: folder dsp ui


folder:
	mkdir -p kuiza.lv2/

install:
	cp -r kuiza.lv2 ~/.lv2/

ui: $(UISOURCES) $(UI)
dsp: $(DSPSOURCES) $(DSP)


$(DSP): $(DSPOBJECTS)
	$(CC) $(DSPOBJECTS) $(INCLUDES) $(CFLAGS) $(LDFLAGS) -o $@

$(UI): $(UIOBJECTS)
	$(CC) $(UIOBJECTS)  $(INCLUDES) $(CFLAGS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $< -g $(CFLAGS) -c -o $@

clean:
	rm -f uiTest
	rm -f kuiza.lv2/kuiza.so
	rm -f kuiza.lv2/kuiza_ui.so

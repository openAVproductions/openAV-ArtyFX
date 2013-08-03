
CC=g++
CFLAGS=-g -Wall

INCLUDES=$(shell pkg-config --cflags sndfile cairomm-1.0 ntk ntk_images)
LDFLAGS=$(shell pkg-config --libs sndfile cairomm-1.0 ntk ntk_images) -fPIC -shared -Wl,-z,nodelete

#UISOURCES=gui/ui_helpers.cxx gui/fabla.cxx gui/fabla_ui.c
#UIOBJECTS=$(UISOURCES:.cpp=.o)
#UI=fabla.lv2/fabla_ui.so

DSPSOURCES=dsp/ducka.cxx
DSPOBJECTS=$(DSPSOURCES:.cpp=.o)
DSP=ducka.lv2/ducka.so

all: folder dsp


folder:
	mkdir -p ducka.lv2/

install:
	cp -r ducka.lv2 ~/.lv2/

#ui: $(UISOURCES) $(UI)
dsp: $(DSPSOURCES) $(DSP)


$(DSP): $(DSPOBJECTS)
	$(CC) $(DSPOBJECTS) $(INCLUDES) $(CFLAGS) $(LDFLAGS) -o $@

$(UI): $(UIOBJECTS)
	$(CC) $(UIOBJECTS)  $(INCLUDES) $(CFLAGS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $< -g $(CFLAGS) -c -o $@

clean:
	rm -f uiTest
	rm -f ducka.lv2/ducka.so
	rm -f ducka.lv2/ducka_ui.so

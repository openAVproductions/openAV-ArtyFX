
CC=g++
CFLAGS=-O3 -Wall

INCLUDES=$(shell pkg-config --cflags sndfile cairomm-1.0 ntk ntk_images)
LDFLAGS=$(shell pkg-config --libs sndfile cairomm-1.0 ntk ntk_images) -fPIC -shared -Wl,-z,nodelete

UISOURCES=gui/widget.cxx gui/ui.cxx
UIOBJECTS=$(UISOURCES:.cpp=.o)
UI=vihda.lv2/vihda_ui.so

DSPSOURCES=dsp/dsp.cxx
DSPOBJECTS=$(DSPSOURCES:.cpp=.o)
DSP=vihda.lv2/vihda.so

all: folder dsp ui


folder:
	mkdir -p vihda.lv2/

install:
	cp -r vihda.lv2 ~/.lv2/

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
	rm -f vihda.lv2/vihda.so
	rm -f vihda.lv2/vihda_ui.so


CC=g++
CFLAGS=-O3 -Wall

INCLUDES=$(shell pkg-config --cflags sndfile cairomm-1.0 ntk ntk_images)
LDFLAGS=$(shell pkg-config --libs sndfile cairomm-1.0 ntk ntk_images) -fPIC -shared -Wl,-z,nodelete

UISOURCES=gui/widget.cxx gui/ui.cxx
UIOBJECTS=$(UISOURCES:.cpp=.o)
UI=satma.lv2/satma_ui.so

DSPSOURCES=dsp/dsp.cxx
DSPOBJECTS=$(DSPSOURCES:.cpp=.o)
DSP=satma.lv2/satma.so

all: folder dsp ui


folder:
	mkdir -p satma.lv2/

install:
	cp -r satma.lv2 ~/.lv2/

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
	rm -f satma.lv2/satma.so
	rm -f satma.lv2/satma_ui.so

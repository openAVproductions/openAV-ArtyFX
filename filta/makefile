INSTALL_PATH=~/.lv2

CC=g++
CFLAGS=-O3 -Wall

INCLUDES=$(shell pkg-config --cflags sndfile)
# INCLUDES=$(shell pkg-config --cflags sndfile cairomm-1.0 ntk ntk_images)
LDFLAGS=$(shell pkg-config --libs sndfile) -fPIC -shared -Wl,-z,nodelete
# LDFLAGS=$(shell pkg-config --libs sndfile cairomm-1.0 ntk ntk_images) -fPIC -shared -Wl,-z,nodelete

UISOURCES=gui/widget.cxx gui/ui.cxx
UIOBJECTS=$(UISOURCES:.cpp=.o)
UI=filta.lv2/filta_ui.so

DSPSOURCES=dsp/dsp.cxx
DSPOBJECTS=$(DSPSOURCES:.cpp=.o)
DSP=filta.lv2/filta.so

all: folder dsp
# all: folder dsp ui


folder:
	mkdir -p filta.lv2/

install:
	cp -r filta.lv2 $(INSTALL_PATH)

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
	rm -f filta.lv2/filta.so
	rm -f filta.lv2/filta_ui.so

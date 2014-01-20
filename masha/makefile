
CC=g++
CFLAGS=-O3 -Wall

INCLUDES=$(shell pkg-config --cflags sndfile cairomm-1.0)
# INCLUDES=$(shell pkg-config --cflags sndfile cairomm-1.0 ntk ntk_images)
LDFLAGS=$(shell pkg-config --libs sndfile cairomm-1.0) -fPIC -shared -Wl,-z,nodelete
# LDFLAGS=$(shell pkg-config --libs sndfile cairomm-1.0 ntk ntk_images) -fPIC -shared -Wl,-z,nodelete

UISOURCES=gui/masha_widget.cxx gui/masha_ui.cxx
UIOBJECTS=$(UISOURCES:.cpp=.o)
UI=masha.lv2/masha_ui.so

DSPSOURCES=dsp/masha.cxx
DSPOBJECTS=$(DSPSOURCES:.cpp=.o)
DSP=masha.lv2/masha.so

all: folder dsp
# all: folder dsp ui


folder:
	mkdir -p masha.lv2/

install:
	cp -r masha.lv2 ~/.lv2/

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
	rm -f masha.lv2/masha.so
	rm -f masha.lv2/masha_ui.so

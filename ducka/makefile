
CC=g++
CFLAGS=-O3 -Wall

INCLUDES=$(shell pkg-config --cflags sndfile cairomm-1.0 ntk ntk_images)
LDFLAGS=$(shell pkg-config --libs sndfile cairomm-1.0 ntk ntk_images) -fPIC -shared -Wl,-z,nodelete

UISOURCES=gui/ducka_widget.cxx gui/ducka_ui.cxx
UIOBJECTS=$(UISOURCES:.cpp=.o)
UI=ducka.lv2/ducka_ui.so

DSPSOURCES=dsp/dsp.cxx
DSPOBJECTS=$(DSPSOURCES:.cpp=.o)
DSP=ducka.lv2/ducka.so

all: folder dsp ui


folder:
	mkdir -p ducka.lv2/

install:
	cp -r ducka.lv2 ~/.lv2/

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
	rm -f ducka.lv2/ducka.so
	rm -f ducka.lv2/ducka_ui.so

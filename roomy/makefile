INSTALL_PATH=~/.lv2

CC=g++
CFLAGS=-O3 -Wall

INCLUDES=$(shell pkg-config --cflags sndfile )
# INCLUDES=$(shell pkg-config --cflags sndfile cairomm-1.0 ntk ntk_images)
LDFLAGS=$(shell pkg-config --libs sndfile ) -fPIC -shared -Wl,-z,nodelete
# LDFLAGS=$(shell pkg-config --libs sndfile cairomm-1.0 ntk ntk_images) -fPIC -shared -Wl,-z,nodelete

UISOURCES=gui/roomy_widget.cxx gui/roomy_ui.cxx
UIOBJECTS=$(UISOURCES:.cpp=.o)
UI=roomy.lv2/roomy_ui.so

DSPSOURCES=dsp/roomy.cxx
DSPOBJECTS=$(DSPSOURCES:.cpp=.o)
DSP=roomy.lv2/roomy.so

all: folder dsp
# all: folder dsp ui


folder:
	mkdir -p roomy.lv2/

install:
	cp -r roomy.lv2 $(INSTALL_PATH)

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
	rm -f roomy.lv2/roomy.so
	rm -f roomy.lv2/roomy_ui.so


all:
	g++  -Wl,-z,nodelete  -Wl,--no-undefined -fPIC -shared -DPUGL_HAVE_CAIRO -I../avtk/ ../avtk/pugl/pugl_x11.c ../avtk/*.cxx testUi.cxx `pkg-config --cflags --libs cairo x11` -o test.lv2/testui.so

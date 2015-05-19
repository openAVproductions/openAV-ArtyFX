
all:
	g++ -shared -fPIC -DPUGL_HAVE_CAIRO -I ../avtk/avtk/ ../avtk/test_ui.cxx ../avtk/avtk/pugl/pugl_x11.c lv2_ui.cxx -Wl,--no-undefined -L../avtk/build/ -lavtk -lcairo -lX11 -o lv2ui.so

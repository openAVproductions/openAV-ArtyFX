
all:
	g++ -shared -fPIC -DPUGL_HAVE_CAIRO -I ../avtk/avtk/ graphs/*.cxx della.cxx roomy.cxx ../avtk/test_ui.cxx ../avtk/avtk/pugl/pugl_x11.c lv2_ui.cxx -Wl,--no-undefined -L../avtk/build/ -lavtk -lcairo -lX11 -o artyfx_ui.so

install:
	cp artyfx_ui.so /usr/local/lib/lv2/artyfx.lv2/

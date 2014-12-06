
all:
	g++ -g -DPUGL_HAVE_CAIRO avtk/pugl/pugl_x11.c avtk/ui.cxx avtk/button.cxx avtk/widget.cxx main.cxx -lGL -lcairo -lX11 -llo -o vizia_ui

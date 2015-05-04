

all:
	g++  -Wl,-z,nodelete  -Wl,--no-undefined -fPIC -shared -DPUGL_HAVE_CAIRO -I../avtk/ ../test_ui.cxx ../avtk/pugl/pugl_x11.c ../avtk/*.cxx ../libs/*.cxx testUi.cxx `pkg-config --cflags --libs cairo x11` -o avtk.lv2/avtk_ui.so

install: all
	cp -r avtk.lv2 ~/.lv2/


dsp:
	faust2lv2 -uri-prefix http://www.openavproductions.com avtk.dsp
	# manually hack the following into the avtk.ttl file
	#
	# @prefix ui: <http://lv2plug.in/ns/extensions/ui#>.
	#
	# <http://www.openavproductions.com/avtk#testUI>
	#		a ui:X11UI;
	#		ui:binary <avtk_ui.so>;
	#		ui:requiredFeature ui:makeResident ;
	#		lv2:requiredFeature ui:idleInterface ;
	#		lv2:extensionData ui:idleInterface .


#include "waveform_test.hxx"

#include "../avtk/theme.hxx"

#include <sstream>

#define WIDTH  420
#define HEIGHT 160

WaveformUI::WaveformUI(PuglNativeWindow parent) :
	Avtk::UI( WIDTH, HEIGHT, parent, "AVTK : Waveform" )
{
	themes.push_back( new Avtk::Theme( this, "default.avtk" ) );
	Widget::theme_ = themes.front();

	Avtk::Widget* w = 0;

	waveform = new Avtk::Waveform( this, 10, 10, WIDTH/2-20, HEIGHT-20, "Waveform" );
}

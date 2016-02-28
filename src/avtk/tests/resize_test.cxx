
#include "resize_test.hxx"

#include "../avtk/theme.hxx"

#include <sstream>

#define WIDTH  640
#define HEIGHT 340

ResizeUI::ResizeUI(PuglNativeWindow parent) :
	Avtk::UI( WIDTH, HEIGHT, parent, "AVTK Test : resize" )
{
	themes.push_back( new Avtk::Theme( this, "default.avtk" ) );
	Widget::theme_ = themes.front();

	Avtk::Widget* w = 0;

	dial = new Avtk::Dial( this, 10, 10, 40, 40, "Dial 1" );

	dial = new Avtk::Dial( this, 80, 80, 40, 40, "Dial 2" );
}



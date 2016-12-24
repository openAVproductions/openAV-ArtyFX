
#include "number_test.hxx"

#include "../avtk/theme.hxx"

#include <sstream>

#define WIDTH  160
#define HEIGHT 60

NumberUI::NumberUI(PuglNativeWindow parent) :
	Avtk::UI( WIDTH, HEIGHT, parent, "AVTK : Number" )
{
	themes.push_back( new Avtk::Theme( this, "default.avtk" ) );
	Widget::theme_ = themes.front();

	Avtk::Widget* w = 0;

	number = new Avtk::Number( this, 10, 10, 20, HEIGHT-20, "1-10" );

	number = new Avtk::Number( this, 80, 10, 20, HEIGHT-20, "4-6" );
	number->valueMode( Avtk::Widget::VALUE_INT, 4, 2 );
}

void NumberUI::widgetValueCB( Avtk::Widget* widget)
{
	float v = widget->value();
	int i = int(v);
	printf("%s got value %f, %i\n", widget->label(), v, i );
}


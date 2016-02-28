
#include "della.hxx"
#include "../dsp/della.hxx"

#include "common.hxx"
#include "theme.hxx"
#include "graphs/delay.hxx"
#include "headers/della.c"

DellaUI::DellaUI(PuglNativeWindow parent) :
	Avtk::UI( WIDTH, HEIGHT, parent, "Della (ArtyFX-OpenAV)" )
{
	Avtk::Image* i = new Avtk::Image( this, 0, 0, 160,  29, "header");
	i->load( della.pixel_data );

	rev   = new Avtk::Delay( this, 5,36, 150, 126, "graph" );

	dial1 = new Avtk::Dial( this,  8, 172, 45,45, "Feedback" );
	dial2 = new Avtk::Dial( this, 60, 172, 45,45, "Volume" );
	dial3 = new Avtk::Dial( this,110, 172, 45,45, "Time" );

	dial1->defaultValue( 0.25 );
	dial2->defaultValue( 0.5 );
	dial3->defaultValue( 0.5 );

	dial1->value( 0.25 );
	dial2->value( 0.5 );
	dial3->value( 0.5 );
}

void DellaUI::widgetValueCB( Avtk::Widget* widget )
{
	float v = widget->value();
	//printf("Widget %s : %f\n", widget->label(), v );
	if( widget == dial1 ) {
		rev->feedback = v;
		write_function( controller, DELLA_FEEDBACK, sizeof(float), 0, &v );
	}
	if( widget == dial2 ) {
		rev->volume = v;
		write_function( controller, DELLA_VOLUME, sizeof(float), 0, &v );
	}
	if( widget == dial3 ) {
		rev->time = v;
		write_function( controller, DELLA_TIME, sizeof(float), 0, &v );
	}
	redraw();
}

void DellaUI::lv2PortEvent( uint32_t index,
                            uint32_t buf_size,
                            uint32_t format,
                            const void* buffer )
{
	if( format != 0 )
		return;

	float v = *((float*)buffer);

	//printf("DellaUI port() %i : v\n", index, v );

	switch( index ) {
	case DELLA_FEEDBACK:
		dial1->value( v );
		rev->feedback = v;
		break;
	case DELLA_VOLUME:
		dial2->value( v );
		rev->volume = v;
		break;
	case DELLA_TIME:
		dial3->value( v );
		rev->time = v;
		break;
	}
	redraw();
	return;
}


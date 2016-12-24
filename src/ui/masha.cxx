
#include "masha.hxx"
#include "../dsp/masha.hxx"

#include "common.hxx"
#include "../avtk/theme.hxx"
#include "../avtk/button.hxx"
#include "graphs/masher.hxx"
#include "headers/masha.c"

MashaUI::MashaUI(PuglNativeWindow parent) :
	Avtk::UI( WIDTH, HEIGHT, parent, "Masha (ArtyFX-OpenAV)" )
{
	Avtk::Image* i = new Avtk::Image( this, 0, 0, 160,  29, "header");
	i->load( masha.pixel_data );

	graph = new Avtk::Masher( this, 5,36, 150, 126, "graph" );
	graph->clickMode( CLICK_TOGGLE );
	graph->rClickMode( RCLICK_NONE );

	dial1 = new Avtk::Dial( this,  40, 172, 39,39, "Time" );
	dial2 = new Avtk::Dial( this, 114, 172, 39,39, "Vol" );
	dial3 = new Avtk::Dial( this,  78, 172, 39,39, "Pass" );
	dial4 = new Avtk::Dial( this,   4, 172, 39,39, "BPM " );
	dial4->visible(false);

	// Optional Connect
	button= new Avtk::Button( this, 4, 146, 65,25, "HostBPM" );
	button->clickMode(CLICK_TOGGLE);
	button->value(true);
}

void MashaUI::widgetValueCB( Avtk::Widget* widget )
{
	float v = widget->value();
	//printf("Widget %s : %f\n", widget->label(), v );
	if( widget == dial1 ) {
		graph->time = v;
		write_function( controller, MASHA_TIME, sizeof(float), 0, &v );
	}
	if( widget == dial2 ) {
		graph->volume = v;
		write_function( controller, MASHA_AMP, sizeof(float), 0, &v );
	}
	if( widget == dial3 ) {
		graph->passthrough = v;
		write_function( controller, MASHA_DRY_WET, sizeof(float), 0, &v );
	}
	if( widget == dial4 ) {
		float bpm = 40 + 200 * v;
		std::stringstream s;
		s << "Bpm " << int(bpm);
		dial4->label( s.str().c_str() );
		write_function( controller, MASHA_BPM_MANUAL, sizeof(float), 0, &bpm );
	}
	if( widget == button ) {
		write_function( controller, MASHA_BPM_FROM_HOST, sizeof(float), 0, &v );
		dial4->visible(!int(v));
	}
	if( widget == graph ) {
		write_function( controller, MASHA_ACTIVE, sizeof(float), 0, &v );
	}
	redraw();
}

void MashaUI::lv2PortEvent( uint32_t index,
                            uint32_t buf_size,
                            uint32_t format,
                            const void* buffer )
{
	if( format != 0 )
		return;

	float v = *((float*)buffer);

	//printf("MashaUI port() %i : v\n", index, v );

	switch( index ) {
	case MASHA_TIME:
		dial1->value( v );
		graph->time = v;
		break;
	case MASHA_AMP:
		dial2->value( v );
		graph->volume = v;
		break;
	case MASHA_DRY_WET:
		dial3->value( v );
		graph->passthrough = v;
		break;
	}
	redraw();
	return;
}


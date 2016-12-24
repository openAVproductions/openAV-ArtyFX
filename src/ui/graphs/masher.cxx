
#include "masher.hxx"
#include "theme.hxx"

using namespace Avtk;

Masher::Masher( Avtk::UI* ui, int x_, int y_, int w_, int h_,
                std::string label_) :
	Widget( ui, x_, y_, w_, h_, label_ ),
	time(0),
	volume(0),
	passthrough(0)
{
}

void Masher::draw( cairo_t* cr )
{
	cairo_save( cr );

	cairo_rectangle( cr, x_, y_, w_, h_);
	cairo_clip( cr );
	theme_->color( cr, BG, 0.4 );
	cairo_rectangle( cr, x_, y_, w_, h_);
	cairo_fill( cr );

	// check that its a new "segment" to redraw
	int newAngle = ( time * 4.9f);

	float angle = 0.f;

	if      ( newAngle == 0 )
		angle = 0.06;
	else if ( newAngle == 1 )
		angle = 0.125;
	else if ( newAngle <= 2 )
		angle = 0.25;
	else if ( newAngle <= 3 )
		angle = 0.5;
	else
		angle = 1;

	// replace: middle arc
	cairo_set_line_cap ( cr, CAIRO_LINE_CAP_ROUND );
	cairo_arc( cr, x_ + w_/2., y_ + h_/2, h_/2 * 6.5 / 13 + 4, -(3.1415/2),
	           passthrough * 6.28 - (3.1415/2) );
	cairo_set_source_rgba (cr, 0.0, 0.318, 1.0, 1 );
	cairo_set_line_width(cr, 7);
	cairo_stroke(cr);

	if      ( newAngle == 0 )
		cairo_set_source_rgba (cr, 1.0, 0.0 , 0.0, 1 );
	else if ( newAngle == 1 )
		cairo_set_source_rgba (cr, 1.0, 0.318, 0.0, 1 );
	else if ( newAngle <= 2 )
		cairo_set_source_rgba (cr, 0.0, 1.0 , 0.0, 0.8 );
	else if ( newAngle <= 3 )
		cairo_set_source_rgba (cr, 0.0, 0.318, 1.0, 1 );
	else
		cairo_set_source_rgba (cr, 1.0, 0.0, 1.0, 0.7 );


	// duration : outside arc
	int xc = w_/2;
	int yc = h_/2;
	cairo_arc( cr, x_ + xc, y_ + yc, xc * 9.5 / 14, -(3.1415/2), angle * 6.28 - (3.1415/2) );
	cairo_set_line_width(cr, 11);
	cairo_stroke(cr);


	// volume: inside circle
	cairo_set_source_rgba(cr, 1, 1, 1, 0.21);
	cairo_arc(cr, x_ + xc, y_ + yc, 25 * volume, 0, 2 * 3.1415);
	cairo_set_line_width(cr, 4.2);
	cairo_fill_preserve(cr);
	cairo_set_source_rgba(cr, 1, 1, 1, 0.9);
	cairo_set_line_width(cr, 1.4);
	cairo_stroke(cr);


	// stroke rim
	cairo_rectangle(cr, x_, y_, w_, h_);
	//cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 1 );
	cairo_set_source_rgba( cr,  126 / 255.f,  126 / 255.f ,  126 / 255.f , 0.8 );
	cairo_set_line_width(cr, 1.0);
	cairo_stroke( cr );

	theme_->color( cr, HIGHLIGHT, 0.8 );

	cairo_restore( cr );
}


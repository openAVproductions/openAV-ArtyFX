
#include "wah.hxx"
#include "theme.hxx"

#include <math.h>

using namespace Avtk;

Wah::Wah( Avtk::UI* ui, int x_, int y_, int w_, int h_,
          std::string label_) :
	Widget( ui, x_, y_, w_, h_, label_ ),
	freq(0),
	drive(0),
	mix(0)
{
}

void Wah::draw( cairo_t* cr )
{
	cairo_save( cr );

	cairo_rectangle( cr, x_, y_, w_, h_);
	cairo_clip( cr );
	theme_->color( cr, BG, 0.4 );
	cairo_rectangle( cr, x_, y_, w_, h_);
	cairo_fill( cr );

	{
		// horizontal line, "base" for wah
		cairo_move_to( cr, x_ + 0, y_ + h_ * 2 / 3. );
		cairo_line_to( cr, x_ + w_, y_ + h_ * 2 / 3. );
		//cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 0.4 );
		theme_->color( cr, HIGHLIGHT, 0.8 );
		cairo_set_line_width( cr, 1.5);
		cairo_close_path( cr );
		cairo_stroke( cr );


		float v = ( freq * 3.1415 ) /2 ;

		float size = drive * 0.8 + 0.4;

		float x1 = - cos( v ) * w_ / 3 * size;
		float y1 = - sin( v ) * h_ / 3 * size;

		float x2 = - cos( v + 3.1415/2 ) * w_ / 3 * size;
		float y2 = - sin( v + 3.1415/2 ) * h_ / 3 * size;

		cairo_move_to( cr, x_+w_/2    , y_+h_ * 2 / 3. );
		cairo_line_to( cr, x_+w_/2+x1 , y_+h_ *2/3+y1 );
		cairo_line_to( cr, x_+w_/2+x2 , y_+h_ *2/3+y2 );
		cairo_close_path( cr );
		cairo_set_line_width( cr, 2.1 );
		cairo_set_line_join( cr, CAIRO_LINE_JOIN_ROUND );
		cairo_set_source_rgba( cr, 1, 1, 1, 0.2 );
		cairo_fill_preserve( cr );
		cairo_set_source_rgba( cr, 1, 1, 1, 0.8 );
		cairo_stroke( cr );

		// mix spikes
		float m = mix * 0.8 + 0.3;
		cairo_move_to( cr, x_+w_*1. /5, y_ + h_ );
		cairo_line_to( cr, x_+w_*1.5/5, y_ + h_ - h_/4.f*m );
		cairo_line_to( cr, x_+w_*2. /5, y_ + h_ );

		cairo_move_to( cr, x_+w_*3. /5, y_ + h_ );
		cairo_line_to( cr, x_+w_*3.5/5, y_ + h_ - h_/4.f*m );
		cairo_line_to( cr, x_+w_*4. /5, y_ + h_ );

		cairo_set_source_rgba( cr, 1, 1, 1, 0.2 );
		cairo_fill_preserve( cr );
		cairo_set_source_rgba( cr, 1, 1, 1, 0.8 );
		cairo_stroke( cr );
	}

	// stroke rim
	cairo_rectangle(cr, x_, y_, w_, h_);
	//cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 1 );
	cairo_set_source_rgba( cr,  126 / 255.f,  126 / 255.f ,  126 / 255.f , 0.8 );
	cairo_set_line_width(cr, 1.0);
	cairo_stroke( cr );

	theme_->color( cr, HIGHLIGHT, 0.8 );

	cairo_restore( cr );
}


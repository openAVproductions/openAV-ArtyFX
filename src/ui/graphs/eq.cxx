
#include "eq.hxx"
#include "theme.hxx"

using namespace Avtk;

Eq::Eq( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
	Widget( ui, x_, y_, w_, h_, label_ )
{
	value( 0.5 );
	rClickMode( RCLICK_NONE );
	for(int i = 0; i < 4; i++)
		gains[i] = 0.5f;
}

void Eq::draw( cairo_t* cr )
{
	cairo_save( cr );

	cairo_rectangle( cr, x_, y_, w_, h_);
	cairo_clip( cr );
	theme_->color( cr, BG, 0.4 );
	cairo_rectangle( cr, x_, y_, w_, h_);
	cairo_fill( cr );


	cairo_set_line_width( cr, 18 );
	cairo_set_line_cap( cr, CAIRO_LINE_CAP_ROUND );
	theme_->color( cr, HIGHLIGHT, 0.8 );

	// imported from NTK eq widget
	{
		int x = x_;
		int y = y_;
		int w = w_;
		int h = h_;


		// draw frequency/amplitude rectangles
		for(int i = 0; i < 4; i ++) {
			int startY = y + h/2;
			int heightY = h/4 * ( ( (1-gains[i])*2)-1 );

			cairo_rectangle( cr, x + (w/4)*i, startY, w/4, heightY );
		}

		cairo_set_line_cap( cr, CAIRO_LINE_CAP_ROUND );
		theme_->color( cr, HIGHLIGHT, 0.21 );
		cairo_fill_preserve(cr);
		cairo_set_line_width(cr, 1.8);
		theme_->color( cr, HIGHLIGHT );
		cairo_stroke(cr);

		// draw "master gain line

		cairo_move_to( cr, x  , y + h/2 + (h/4 *-(value()*2-1)) );
		cairo_line_to( cr, x+w, y + h/2 + (h/4 *-(value()*2-1)) );
		cairo_set_line_width(cr, 2.1);
		cairo_set_source_rgba( cr, 255 / 255.f, 0 / 255.f , 0 / 255.f , 1 );
		cairo_stroke(cr);

	}

	// outline
	cairo_set_line_width( cr, 1 );
	cairo_rectangle( cr, x_, y_, w_, h_ );
	cairo_set_source_rgb( cr, 0.72, 0.72, 0.72 );
	cairo_stroke( cr );

	cairo_restore( cr );
}


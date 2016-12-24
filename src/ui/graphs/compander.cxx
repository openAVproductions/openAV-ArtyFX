
#include "compander.hxx"
#include "theme.hxx"

using namespace Avtk;

Compander::Compander( Avtk::UI* ui, int x_, int y_, int w_, int h_,
                      std::string label_) :
	Widget( ui, x_, y_, w_, h_, label_ )
{
	//dragMode( DM_DRAG_VERTICAL );
	//scrollDisable = false;
	//printf("Avtk UI Compander()\n");
}

void Compander::draw( cairo_t* cr )
{
	cairo_save( cr );

	cairo_rectangle( cr, x_, y_, w_, h_);
	cairo_clip( cr );
	theme_->color( cr, BG, 0.4 );
	cairo_rectangle( cr, x_, y_, w_, h_);
	cairo_fill( cr );

	// Arrow line setup
	cairo_set_line_width( cr, 1 );
	cairo_set_line_cap( cr, CAIRO_LINE_CAP_ROUND );
	cairo_set_line_join( cr, CAIRO_LINE_JOIN_ROUND );

	// fuse NTK panda with AVTK
	float point = threshold;
	{
		// release line
		cairo_move_to( cr, x_ + w_/4, y_ + h_*3.6/4 );
		cairo_line_to( cr, x_ + w_/4 + w_/2*release, y_ + h_*3.6/4 );
		cairo_set_source_rgba(cr, 1.0, 0.48,   0, 0.9);
		cairo_set_line_width( cr, 2.4 );
		cairo_stroke( cr );
		// dickie-bow graph
		int pointX = x_ + w_/4 + w_/2*point;
		int pointY = y_ + h_*3/4 - h_/2*point;

		// lower attenuation arc
		cairo_move_to( cr, x_, y_ + h_ );
		cairo_set_line_width( cr, 1.9 );

		// right curve, compression
		cairo_move_to( cr, pointX, pointY );
		cairo_arc_negative( cr, pointX, pointY, 20 + 18 * (1-point),
		                    0, -3.1415 / 2 * value() );
		cairo_close_path( cr );
		cairo_set_source_rgba(cr, 1.0, 0.318,   0, 0.2);
		cairo_fill_preserve( cr );
		cairo_set_source_rgba(cr, 1.0, 0.318,   0, 0.8);
		cairo_stroke( cr );

		// left curve, expanding
		cairo_move_to( cr, pointX, pointY );
		cairo_arc_negative( cr, pointX, pointY, 20 + 18 * point,
		                    3.1415, 3.1415 + -3.1415 / 2 * value() );
		cairo_close_path( cr );

		cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 0.2 );
		cairo_fill_preserve( cr );
		cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 0.8 );
		cairo_stroke( cr );

		// point
		cairo_arc( cr, pointX, pointY, 6, 0, 6.28 );
		cairo_set_source_rgba( cr, 0, 0, 0, 0.8 );
		cairo_fill_preserve(cr);
		cairo_set_source_rgba( cr, 0, 0, 0 , 1 );
		cairo_set_line_width(cr, 1.5);
		cairo_set_line_join( cr, CAIRO_LINE_JOIN_ROUND);
		cairo_set_line_cap ( cr, CAIRO_LINE_CAP_ROUND);
		cairo_stroke( cr );
	}

	// outline
	cairo_set_line_width( cr, 1 );
	cairo_rectangle( cr, x_, y_, w_, h_ );
	cairo_set_source_rgb( cr, 0.72, 0.72, 0.72 );
	cairo_stroke( cr );


	cairo_restore( cr );
}


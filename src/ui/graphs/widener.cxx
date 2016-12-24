
#include "widener.hxx"
#include "theme.hxx"

using namespace Avtk;

Widener::Widener( Avtk::UI* ui, int x_, int y_, int w_, int h_,
                  std::string label_) :
	Widget( ui, x_, y_, w_, h_, label_ )
{
	//dragMode( DM_DRAG_VERTICAL );
	//scrollDisable = false;
	//printf("Avtk UI Widener()\n");
}

void Widener::draw( cairo_t* cr )
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

	// left arrow
	{
		cairo_move_to( cr, -4+x_+w_/2             , y_+h_/2 );
		cairo_line_to( cr, -4+x_+w_/2-w_/3*width  , y_+h_/2);
		cairo_line_to( cr, -4+x_+w_/2-w_/3*width  , y_+h_/2-9);   // top
		cairo_line_to( cr, -4+x_+w_/2-w_/3*width-9, y_+h_/2+3); // left
		cairo_line_to( cr, -4+x_+w_/2-w_/3*width  , y_+h_/2+15);   // low
		cairo_line_to( cr, -4+x_+w_/2-w_/3*width  , y_+h_/2+6);
		cairo_line_to( cr, -4+x_+w_/2             , y_+h_/2+6);
		cairo_close_path( cr );
	}

	if( invert ) {
		cairo_set_source_rgb( cr, 1,1,1 );
	} else {
		theme_->color( cr, HIGHLIGHT, 1 );
		cairo_fill_preserve( cr );
	}
	cairo_stroke( cr );

	// right arrow
	{
		cairo_move_to( cr, x_+w_/2, y_+h_/2 );
		cairo_line_to( cr, x_+w_/2+w_/3*width  , y_+h_/2);
		cairo_line_to( cr, x_+w_/2+w_/3*width  , y_+h_/2-9);   // top
		cairo_line_to( cr, x_+w_/2+w_/3*width+9, y_+h_/2+3);   // right
		cairo_line_to( cr, x_+w_/2+w_/3*width  , y_+h_/2+15);  // low
		cairo_line_to( cr, x_+w_/2+w_/3*width  , y_+h_/2+6);
		cairo_line_to( cr, x_+w_/2, y_+h_/2+6);
		cairo_close_path( cr );
	}
	if( !invert ) {
		cairo_set_source_rgb( cr, 1,1,1 );
	} else {
		theme_->color( cr, HIGHLIGHT, 1 );
		cairo_fill_preserve( cr );
	}
	cairo_stroke( cr );

	// outline
	cairo_set_line_width( cr, 1 );
	cairo_rectangle( cr, x_, y_, w_, h_ );
	cairo_set_source_rgb( cr, 0.72, 0.72, 0.72 );
	cairo_stroke( cr );


	cairo_restore( cr );
}



#include "box.hxx"

#include "ui.hxx"
#include "theme.hxx"

#include <stdio.h>

using namespace Avtk;

Box::Box( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
	Widget( ui, x_, y_, w_, h_, label_ )
{
	clickMode( CLICK_MOMENTARY );
}

void Box::draw( cairo_t* cr )
{
	cairo_save( cr );


	// draw dark BG
	cairo_rectangle( cr, x_, y_, w_, h_ );
	//roundedBox(cr, x_, y_, w_, h_, theme_->cornerRadius_ );
	theme_->color( cr, BG_DARK, 1 );
	cairo_fill_preserve( cr );
	cairo_stroke( cr );
	theme_->color( cr, BG_DARK, 0.8 );
	cairo_select_font_face(cr, "impact", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);

	// draw header
	cairo_rectangle( cr, x_, y_, w_, 14 );
	theme_->color( cr, HIGHLIGHT, 0.8 );
	cairo_fill( cr );

	// show the text
	cairo_text_extents_t extents;
	cairo_set_font_size(cr, 10.0);
	cairo_text_extents(cr, label(), &extents);
	cairo_move_to(cr, x_ + 4, y_ + 7 + (extents.height / 2) );
	theme_->color( cr, BG_DARK, 1 );
	cairo_show_text( cr, label() );

	cairo_restore( cr );
}


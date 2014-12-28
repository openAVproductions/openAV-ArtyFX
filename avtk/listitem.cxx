
#include "listitem.hxx"

#include "ui.hxx"
#include "theme.hxx"

#include <stdio.h>

using namespace Avtk;

ListItem::ListItem( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ )
{
  clickMode( CLICK_TOGGLE );
}

void ListItem::draw( cairo_t* cr )
{
  cairo_save( cr );
  
  roundedBox(cr, x_, y_, w_, h_, 0 );
  theme_->color( cr, BG, 0.4 );
  cairo_fill(cr);
  
  if( value() > 0.4999 )
  {
      cairo_rectangle( cr, x_, y_, w_, h_ );
      theme_->color( cr, HIGHLIGHT, 1 );
      cairo_fill_preserve( cr );
      cairo_stroke( cr );
      theme_->color( cr, BG_DARK, 0.8 );
      cairo_select_font_face(cr, "impact", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  }
  else
  {
      cairo_select_font_face(cr, "impact", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
      cairo_set_source_rgba( cr, 1,1,1, 0.8 );
  }
  
  // Draw label
  cairo_text_extents_t extents;
  cairo_set_font_size(cr, 10.0);
  cairo_text_extents(cr, label(), &extents);
  cairo_move_to(cr, x_ + 4, (y_ + h_ / 2) + extents.height / 2 - 1.1 );
  
  cairo_show_text( cr, label() );
  
  cairo_restore( cr );
}


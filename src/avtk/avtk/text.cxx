
#include "text.hxx"

#include "ui.hxx"
#include "theme.hxx"

#include <stdio.h>

using namespace Avtk;

Text::Text( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ )
{
  clickMode( CLICK_NONE );
}

void Text::draw( cairo_t* cr )
{
  cairo_save( cr );
  
  // Draw label
  cairo_set_font_size(cr, 10.0);
  
  cairo_move_to(cr, x_ + 4, y_ + 4 );
  
  if( false ) // center?
  {
    cairo_text_extents_t extents;
    cairo_text_extents(cr, label(), &extents);
    cairo_move_to(cr,
                  (x_ + w_ / 2) - extents.width / 2,
                  (y_ + h_ / 2) + extents.height / 2 - 2);
  }
  cairo_set_source_rgb( cr, 1,1,1 );
  cairo_show_text( cr, label() );
  
  cairo_restore( cr );
}


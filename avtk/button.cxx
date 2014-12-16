
#include "button.hxx"

#include "ui.hxx"
#include "theme.hxx"

#include <stdio.h>

using namespace Avtk;

Button::Button( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ )
{
}

void Button::draw( cairo_t* cr )
{
  cairo_save( cr );
  
  roundedBox(cr, x, y, w, h, theme_->cornerRadius_ );
  
  if( value() > 0.4999 )
  {
    theme_->color( cr, HIGHLIGHT, 0.2 );
    cairo_fill_preserve(cr);
    theme_->color( cr, HIGHLIGHT );
    cairo_set_line_width(cr, theme_->lineWidthWide() );
    cairo_stroke(cr);
  }
  else
  {
    theme_->color( cr, BG_DARK );
    cairo_fill_preserve(cr);
    theme_->color( cr, FG );
    cairo_set_line_width(cr, theme_->lineWidthNorm() );
    cairo_stroke(cr);
  }
  
  // Draw label
  cairo_text_extents_t extents;
  cairo_set_font_size(cr, 15.0);
  cairo_text_extents(cr, label(), &extents);
  cairo_move_to(cr,
                (x + w / 2) - extents.width / 2,
                (y + h / 2) + extents.height / 2 - 2);
  
  if( !value() )
  {
    theme_->color( cr, FG );
  }
  else
  {
    theme_->color( cr, BG_DARK );
  }
  cairo_show_text( cr, label() );
  
  cairo_restore( cr );
}



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
  
  roundedBox(cr, x, y, w, h, ui->theme->cornerRadius_ );
  
  if( value() > 0.4999 )
  {
    ui->theme->color( cr, HIGHLIGHT, 0.2 );
    cairo_fill_preserve(cr);
    ui->theme->color( cr, HIGHLIGHT );
    cairo_set_line_width(cr, ui->theme->lineWidthWide() );
    cairo_stroke(cr);
  }
  else
  {
    ui->theme->color( cr, BG_DARK );
    cairo_fill_preserve(cr);
    ui->theme->color( cr, FG );
    cairo_set_line_width(cr, ui->theme->lineWidthNorm() );
    cairo_stroke(cr);
  }
  
  // Draw label
  cairo_text_extents_t extents;
  cairo_set_font_size(cr, 15.0);
  cairo_text_extents(cr, label.c_str(), &extents);
  cairo_move_to(cr,
                (x + w / 2) - extents.width / 2,
                (y + h / 2) + extents.height / 2 - 2);
  
  if( !value() )
  {
    ui->theme->color( cr, FG );
  }
  else
  {
    ui->theme->color( cr, BG_DARK );
  }
  cairo_show_text(cr, label.c_str());
  
  cairo_restore( cr );
}



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
  if ( value() )
  {
    ui->theme->alpha( 0.8 );
    ui->theme->highlight( cr );
  }
  else
  {
    ui->theme->alpha( 0.2 );
    ui->theme->fg( cr );
  }
  
  roundedBox(cr, x, y, w, h, ui->theme->cornerRadius() );
  cairo_fill_preserve(cr);
  
  ui->theme->transparent( false );
  
  cairo_set_line_width(cr, 0.7);
  cairo_stroke(cr);
  
  // Draw label
  cairo_text_extents_t extents;
  cairo_set_font_size(cr, 15.0);
  cairo_text_extents(cr, label.c_str(), &extents);
  cairo_move_to(cr,
                (x + w / 2) - extents.width / 2,
                (y + h / 2) + extents.height / 2 - 2);
  
  if( !value() )
  {
    ui->theme->fg( cr );
  }
  else
  {
    ui->theme->bgDark( cr );
  }
  cairo_show_text(cr, label.c_str());
}


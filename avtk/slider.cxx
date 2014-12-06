
// code aquisited from pugl_cairo_test.c, author David Robillard.

#include "slider.hxx"

#include <stdio.h>

using namespace Avtk;

Slider::Slider(int x_, int y_, int w_, int h_, std::string label_) :
  Widget( x_, y_, w_, h_, label_ )
{
}

void Slider::draw( cairo_t* cr )
{
  if ( value() )
  {
    theme->alpha( 0.4 );
    theme->highlight( cr );
  }
  else
  {
    theme->alpha( 0.2 );
    theme->fg( cr );
  }
  
  roundedBox(cr, x, y, w, h, theme->cornerRadius() );
  cairo_fill_preserve(cr);
  
  theme->transparent( false );
  
  // Draw border
  theme->fg( cr );
  
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
    theme->fg( cr );
  }
  else
  {
    theme->bgDark( cr );
  }
  cairo_show_text(cr, label.c_str());
}


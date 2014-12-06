
// code aquisited from pugl_cairo_test.c, author David Robillard.

#include "button.hxx"

#include <stdio.h>

using namespace Avtk;

Slider::Slider(int x_, int y_, int w_, int h_, std::string label_) :
  Widget( x_, y_, w_, h_, label_ )
{
}

void roundedBox(cairo_t* cr, double x, double y, double w, double h)
{
  static const double radius  = 4;
  static const double degrees = 3.14159265 / 180.0;

  cairo_new_sub_path(cr);
  cairo_arc(cr,
            x + w - radius,
            y + radius,
            radius, -90 * degrees, 0 * degrees);
  cairo_arc(cr,
            x + w - radius, y + h - radius,
            radius, 0 * degrees, 90 * degrees);
  cairo_arc(cr,
            x + radius, y + h - radius,
            radius, 90 * degrees, 180 * degrees);
  cairo_arc(cr,
            x + radius, y + radius,
            radius, 180 * degrees, 270 * degrees);
  cairo_close_path(cr);
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
  
  roundedBox(cr, x, y, w, h);
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


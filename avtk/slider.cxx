
// code aquisited from pugl_cairo_test.c, author David Robillard.

#include "slider.hxx"

#include <stdio.h>
#include "ui.hxx"
#include "theme.hxx"


using namespace Avtk;

Slider::Slider( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ )
{
  clickDragMode( CDM_DRAG_VERTICAL );
}

void Slider::draw( cairo_t* cr )
{
  ui->theme->transparent( false );
  ui->theme->fg( cr );
  roundedBox(cr, x, y, w, h, ui->theme->cornerRadius() );
  cairo_stroke(cr);
  
  // fader
  ui->theme->transparent( true );
  ui->theme->highlight( cr );
  const int faderHeight = 16;
  const int range = (h-faderHeight-2);
  roundedBox(cr, x + 1, y + 1 + range - range*value(), w - 2, faderHeight, ui->theme->cornerRadius() );
  cairo_fill_preserve(cr);
  ui->theme->transparent( false );
  cairo_set_line_width(cr, 1.2);
  cairo_stroke(cr);
}


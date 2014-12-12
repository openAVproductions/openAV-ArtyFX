
#include "slider.hxx"

#include <stdio.h>
#include "ui.hxx"
#include "theme.hxx"


using namespace Avtk;

Slider::Slider( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ )
{
  dragMode( DM_DRAG_VERTICAL );
  
  if ( w_ > h_ )
    dragMode( DM_DRAG_HORIZONTAL );
  
}

void Slider::draw( cairo_t* cr )
{
  static const int faderHeight = 16;
  
  roundedBox(cr, x, y, w, h, ui->theme->cornerRadius_ );
  ui->theme->color( cr, BG_DARK );
  cairo_fill_preserve(cr);
  ui->theme->color( cr, FG );
  cairo_stroke(cr);
  
  // fader
  if( dm == DM_DRAG_VERTICAL )
  {
    const int range = (h-faderHeight-2);
    roundedBox(cr, x + 1, y + 1 + range - range*value(), w - 2, faderHeight, ui->theme->cornerRadius_ );
  }
  else
  {
    const int range = (w-faderHeight-2);
    roundedBox(cr, x + 1 + range*value(), y + 1, faderHeight, h - 2, ui->theme->cornerRadius_ );
  }
  
  ui->theme->color( cr, HIGHLIGHT, 0.2 );
  cairo_fill_preserve(cr);
  ui->theme->color( cr, HIGHLIGHT );
  cairo_set_line_width(cr, 2.2);
  cairo_stroke(cr);
}


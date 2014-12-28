
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
  
  scrollDisable = false;
}

void Slider::draw( cairo_t* cr )
{
  static const int faderHeight = 16;
  
  roundedBox(cr, x_, y_, w_, h_, theme_->cornerRadius_ );
  theme_->color( cr, BG_DARK );
  cairo_fill_preserve(cr);
  theme_->color( cr, FG );
  cairo_stroke(cr);
  
  // fader
  if( dragMode() == DM_DRAG_VERTICAL )
  {
    const int range = (h_-faderHeight-2);
    roundedBox(cr, x_+ 1, y_ + 1 + range - range*value(), w_ - 2, faderHeight, theme_->cornerRadius_ );
  }
  else
  {
    const int range = (w_-faderHeight-2);
    roundedBox(cr, x_ + 1 + range*value(), y_ + 1, faderHeight, h_ - 2, theme_->cornerRadius_ );
  }
  
  theme_->color( cr, HIGHLIGHT, 0.2 );
  cairo_fill_preserve(cr);
  theme_->color( cr, HIGHLIGHT );
  cairo_set_line_width(cr, 1.2);
  cairo_stroke(cr);
}



#include "dial.hxx"

#include <stdio.h>
#include "ui.hxx"
#include "theme.hxx"


using namespace Avtk;

Dial::Dial( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ )
{
  dragMode( DM_DRAG_VERTICAL );
  scrollDisable = false;
}

void Dial::draw( cairo_t* cr )
{
  cairo_save( cr );
  
  theme_->color( cr, FG, 0.5 );
  cairo_new_sub_path( cr );
  cairo_arc(cr, x_+w_/2,y_+h_/2,  w_/2.f - 8, 2.46, 2.46 + 4.54 );
  cairo_set_line_width(cr, w_ / 20.f);
  cairo_stroke(cr);
  
  cairo_new_sub_path( cr );
  cairo_arc(cr, x_+w_/2,y_+h_/2, w_/2.f - 8, 2.46, 2.46 + 4.54*value() );
  
  theme_->color( cr, HIGHLIGHT, 0.2 );
  //cairo_fill_preserve(cr);
  theme_->color( cr, HIGHLIGHT, 0.8 );
  cairo_set_line_width(cr, w_ / 7.f);
  cairo_stroke(cr);
  
  cairo_restore( cr );
}


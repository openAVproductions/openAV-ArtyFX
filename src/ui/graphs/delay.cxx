
#include "delay.hxx"
#include "theme.hxx"

using namespace Avtk;

Delay::Delay( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ )
{
  time = 0.5;
  volume = 1.0;
  feedback = 0.f;
  //dragMode( DM_DRAG_VERTICAL );
  //scrollDisable = false;
  //printf("Avtk UI Delay()\n");
}

void Delay::draw( cairo_t* cr )
{
  cairo_save( cr );
 
  float delay = 0;
  int timeQuantize = int(time*3.999);
  if( timeQuantize == 0 ) delay = 0.125;
  if( timeQuantize == 1 ) delay = 0.250;
  if( timeQuantize == 2 ) delay = 0.500;
  if( timeQuantize == 3 ) delay = 1.00f;

  cairo_move_to( cr, x_ + w_/4, y_+h_-2 );
  cairo_line_to( cr, x_ + w_/4, y_+h_- h_*0.75 );
  
  cairo_set_line_width( cr, 18 );
  cairo_set_line_cap( cr, CAIRO_LINE_CAP_ROUND );
  cairo_set_source_rgba( cr, 0/255., 153/255.f, 1, 0.21 );
  cairo_stroke( cr );
  /*
  cairo_move_to( cr, x_, y_ + h_ );
  cairo_line_to( cr, x_+w_*0.1, y_+h_*0.85 - (h_*0.7*dryWet) );
  cairo_line_to( cr, x_+w_*0.3+w_*0.7*size, y_+(h_*0.99) );
  
  if( true ) // active 
    theme_->color( cr, HIGHLIGHT, 0.2 );
  else
    theme_->color( cr, BG_DARK, 0.5 );
  cairo_fill_preserve( cr );
  theme_->color( cr, HIGHLIGHT, 0.8 );
  cairo_stroke( cr );

  // draw "damping" line
  int tmpY = y_+h_*0.85 - (h_*0.7*dryWet);
  cairo_move_to( cr, x_+w_*0.1, tmpY );
  cairo_line_to( cr, x_+w_*0.1+ damping*w_*0.7, tmpY );
  cairo_set_source_rgb( cr, 1,1,1 );
  cairo_stroke( cr );
 
  cairo_rectangle( cr, x_, y_, w_, h_ );
  theme_->color( cr, FG );
  cairo_stroke( cr );
  */

  cairo_restore( cr );
}


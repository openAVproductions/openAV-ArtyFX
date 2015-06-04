
#include "bitcrusher.hxx"
#include "theme.hxx"

using namespace Avtk;

Bitcrusher::Bitcrusher( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ )
{ 
  //dragMode( DM_DRAG_VERTICAL );
  //scrollDisable = false;
  //printf("Avtk UI Bitcrusher()\n");
}

void Bitcrusher::draw( cairo_t* cr )
{
  cairo_save( cr );
  
  cairo_rectangle( cr, x_, y_, w_, h_); 
  cairo_clip( cr );
  theme_->color( cr, BG, 0.4 );
  cairo_rectangle( cr, x_, y_, w_, h_); 
  cairo_fill( cr );

  
  cairo_save( cr );
  
  cairo_set_line_width( cr, 18 );
  cairo_set_line_cap( cr, CAIRO_LINE_CAP_ROUND );
  theme_->color( cr, HIGHLIGHT, 0.8 );
  //cairo_set_source_rgba( cr, 0/255., 153/255.f, 1, 0.821 );

  float bitcrusher = 0;
  int timeQuantize = int(value()*3.999);
  if( timeQuantize == 0 ) bitcrusher = 0.125;
  if( timeQuantize == 1 ) bitcrusher = 0.250;
  if( timeQuantize == 2 ) bitcrusher = 0.500;
  if( timeQuantize == 3 ) bitcrusher = 1.00f;
  
  // audio bar
  cairo_move_to( cr, x_ + w_/4, y_+h_-2 );
  cairo_line_to( cr, x_ + w_/4, y_+h_- h_*0.75 );
  cairo_stroke( cr );


  /*
  // thin red line
  cairo_save( cr );
  cairo_set_source_rgb( cr, 1, 0, 0 ); 
  cairo_set_line_width( cr, 1.5 );
  // feedback pointer
  cairo_move_to( cr, x_+w_/4 + w_/2*value(), y_+h_*3.5/4 );
  cairo_line_to( cr, x_+w_*3.5/4        , y_+h_*3.5/4 );
  cairo_line_to( cr, x_+w_*3.5/4        , y_+h_*1.0/4 );

  int w2fb = (w_/2.f)*feedback;
  cairo_line_to( cr, x_+w_*3.5/4 - w2fb, y_ + h_ * 0.25 );
  cairo_stroke( cr ); // new path for arrowhead

  // arrowhead
  cairo_line_to( cr, x_+w_*3.5/4 - w2fb -10, y_ + h_ * 0.25     );
  cairo_line_to( cr, x_+w_*3.5/4 - w2fb - 2, y_ + h_ * 0.25 + 8 );
  cairo_line_to( cr, x_+w_*3.5/4 - w2fb - 2, y_ + h_ * 0.25 - 8 );
  cairo_close_path( cr );
  cairo_stroke( cr );
  cairo_restore( cr );
  */
  // bitcrusher bar
  cairo_move_to( cr, x_+w_/4+w_/2*bitcrusher, y_+h_-2 );
  cairo_line_to( cr, x_+w_/4+w_/2*bitcrusher, y_+h_- h_*0.5*(1*0.2) );
  cairo_stroke( cr );
  /* 
  cairo_restore( cr );
  
  // outline
  cairo_rectangle( cr, x_, y_, w_, h_ );
  theme_->color( cr, BG );
  cairo_set_line_width( cr, 4.1 );
  cairo_stroke( cr );
  
  
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


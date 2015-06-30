
#include "reverb.hxx"
#include "theme.hxx"

using namespace Avtk;

Reverb::Reverb( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ )
{
  dragMode( DM_DRAG_VERTICAL );
  scrollDisable = false;
}

void Reverb::draw( cairo_t* cr )
{
  cairo_save( cr );
  
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

  // outline
  cairo_set_line_width( cr, 1 );
  cairo_rectangle( cr, x_, y_, w_, h_ );
  cairo_set_source_rgb( cr, 0.72, 0.72, 0.72 );
  cairo_stroke( cr );

  cairo_restore( cr );
}


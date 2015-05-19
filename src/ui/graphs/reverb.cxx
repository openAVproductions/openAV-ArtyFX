
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
    theme_->color( cr, FG, 0.5 );
  else
    theme_->color( cr, BG_DARK, 0.5 );
  cairo_fill_preserve( cr );
  cairo_stroke( cr );


   
  /*
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
 
  if( true ) // show label?
  {
    cairo_move_to( cr, x_, y_ );
    cairo_show_text( cr, label() );
  }
  */

  cairo_restore( cr );
}


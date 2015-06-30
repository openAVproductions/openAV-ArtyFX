
#include "dial.hxx"

#include <stdio.h>
#include "ui.hxx"
#include "theme.hxx"


using namespace Avtk;

Dial::Dial( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string l) :
  Widget( ui, x_, y_, w_, h_, l )
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
 
  if( true ) // show label?
  {
    cairo_text_extents_t ext;
    cairo_text_extents( cr, label(), &ext );

    cairo_move_to( cr, x_+w_/2-ext.width/2., y_+h_+ext.height/2.-4);
    cairo_set_source_rgb( cr, 1,1,1 );
    cairo_show_text( cr, label() );
  }

  cairo_restore( cr );
}


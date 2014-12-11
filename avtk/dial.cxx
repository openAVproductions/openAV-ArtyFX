
#include "dial.hxx"

#include <stdio.h>
#include "ui.hxx"
#include "theme.hxx"


using namespace Avtk;

Dial::Dial( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ )
{
  clickDragMode( CDM_DRAG_VERTICAL );
  
  /*
  if ( w_ > h_ )
    clickDragMode( CDM_DRAG_HORIZONTAL );
  */
}

void Dial::draw( cairo_t* cr )
{
  cairo_save( cr );
  
  /*
  // attempt 1: outside, inside static, pointer
  ui->theme->color( cr, FG, 0.8 );
  cairo_new_sub_path( cr );
  cairo_arc(cr, x+w/2,y+h/2,  w/2.f - 8, 2.46, 2.46 + 4.54 );
  cairo_set_line_width(cr, 1.2);
  cairo_new_sub_path( cr );
  cairo_stroke(cr);
  
  cairo_new_sub_path( cr );
  cairo_arc_negative(cr, x+w/2,y+h/2,  w/2.f, 2.46 + 4.54*value(), 2.46  );
  cairo_move_to( cr, x+w/2,y+h/2 );
  cairo_arc_negative(cr, x+w/2,y+h/2,  w/2.f-12, 2.46 + 4.54*value(), 2.46 + 4.54*value()-0.01  );
  */
  
  // attempt 2: 
  ui->theme->color( cr, BG, 1 );
  cairo_new_sub_path( cr );
  cairo_arc(cr, x+w/2,y+h/2,  w/2.f - 8, 2.46, 2.46 + 4.54 );
  cairo_set_line_width(cr, w / 20.f);
  cairo_stroke(cr);
  
  cairo_new_sub_path( cr );
  cairo_arc(cr, x+w/2,y+h/2,  w/2.f - 8, 2.46, 2.46 + 4.54*value() );
  
  ui->theme->color( cr, HIGHLIGHT, 0.2 );
  //cairo_fill_preserve(cr);
  ui->theme->color( cr, HIGHLIGHT, 0.8 );
  cairo_set_line_width(cr, w / 7.f);
  cairo_stroke(cr);
  
  cairo_restore( cr );
}


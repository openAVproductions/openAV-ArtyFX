
#include "envelope.hxx"

#include <stdio.h>
#include "ui.hxx"
#include "theme.hxx"


using namespace Avtk;

Envelope::Envelope( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ )
{
  dragMode( DM_DRAG_VERTICAL );
  
  a = 0.f;
  d = s = r = 0.5f;
}

void Envelope::draw( cairo_t* cr )
{
  cairo_save( cr );
  
  roundedBox(cr, x, y, w, h, theme_->cornerRadius_ );
  theme_->color( cr, BG_DARK );
  cairo_fill_preserve(cr);
  theme_->color( cr, FG );
  cairo_set_line_width(cr, 1.4);
  cairo_stroke(cr);
  
  a = d = s = r = value();
  
  cairo_move_to( cr, x + 2, y + h );
  cairo_line_to( cr, x + 5 + (w * (a / 5.f)), y + h * 0.1 ); // attack
  cairo_rel_line_to( cr, w * (d / 5.2f),   (h*0.9) * s    ); // decay, and sustain height
  cairo_rel_line_to( cr, w * 0.4, 0 );                       // sustain horizontal line
  cairo_rel_line_to( cr, 0.85 * w * ( (r) / 5.f), h - (h*0.9) * s - h * 0.1  ); // Y down
  
  cairo_set_line_join( cr, CAIRO_LINE_JOIN_ROUND);
  theme_->color( cr, HIGHLIGHT, 0.2 );
  cairo_fill_preserve( cr );
  
  theme_->color( cr, HIGHLIGHT, 0.8 );
  cairo_set_line_width(cr, 2.0);
  cairo_stroke(cr);
  
  cairo_restore( cr );
}


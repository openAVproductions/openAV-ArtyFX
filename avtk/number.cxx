
#include "number.hxx"

#include <stdio.h>
#include <sstream>
#include "ui.hxx"
#include "theme.hxx"


using namespace Avtk;

Number::Number( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ ),
  base( 1 ),
  amount( 9 ),
  blank( 0 )
{
  dragMode( DM_DRAG_VERTICAL );
  setScrollDeltaAmount( amount ); // interger counting
}

void Number::setRange( int b, int a )
{
  base = b;
  amount = a;
  setScrollDeltaAmount( amount ); // interger counting
  ui->redraw();
}

void Number::blankValue( int b )
{
  blank = b;
}

void Number::draw( cairo_t* cr )
{
  cairo_save( cr );
  
  theme_->color( cr, HIGHLIGHT, 0.8 );
  roundedBox(cr, x, y, w, h, theme_->cornerRadius_ );
  cairo_fill_preserve(cr);
  theme_->color( cr, BG_DARK, 1 );
  cairo_set_line_width(cr, theme_->lineWidthWide() );
  cairo_stroke(cr);
  
  // single int digit shown
  int v = base + (value() * amount);
  
  std::stringstream vStr;
  if( v != blank )
    vStr << v;
  else
    vStr << '_';
  
  int offset = 0;
  if( v == 10 )
    offset = 0; // move 2 px left, looks better for 2 digits
  
  // Draw label
  cairo_text_extents_t extents;
  cairo_set_font_size(cr, 15.0);
  
  cairo_select_font_face(cr, "impact", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  
  cairo_text_extents(cr, vStr.str().c_str(), &extents);
  cairo_move_to(cr, (x + w / 2) - extents.width / 2 + offset,
                    (y + h / 2) + extents.height / 2 );
  
  theme_->color( cr, BG_DARK );
  cairo_show_text( cr, vStr.str().c_str() );
  
  cairo_restore( cr );
}


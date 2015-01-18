
#include "dialog.hxx"

#include "button.hxx"
#include "ui.hxx"
#include "theme.hxx"

#include <stdio.h>

using namespace Avtk;

Dialog::Dialog( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Group( ui, x_, y_, w_, h_, label_ )
{
  returnVal = 0;
  
  ok     = new Button( ui, 0, 0, 60, 20, "OK" );
  cancel = new Button( ui, 0, 0, 60, 20, "Cancel" );
  
  add( ok     );
  add( cancel );
  
  visible_ = false;
}

void Dialog::draw( cairo_t* cr )
{
  cairo_save( cr );
  
  // transparent-out the *entire* UI
  cairo_rectangle( cr, 0, 0, ui->w(), ui->h() );
  //theme_->color( cr, BG_DARK, 0.8 );
  cairo_set_source_rgba( cr, 0, 0, 0 , 0.5 );
  cairo_fill_preserve( cr );
  cairo_stroke( cr );
  
  // draw diagonal lines
  int end = x_ + w_ * 2;
  for(int i = 0; i < end; i += 40 )
  {
    cairo_move_to( cr,    -10,  i + 10 );
    cairo_line_to( cr, i + 10,     -10 );
  }
  theme_->color( cr, BG, 0.4 );
  cairo_set_line_width( cr, 15 );
  cairo_stroke( cr );
  
  // draw dark BG
  cairo_rectangle( cr, x_, y_, w_, h_ );
  theme_->color( cr, BG_DARK, 1 );
  cairo_fill_preserve( cr );
  cairo_stroke( cr );
  theme_->color( cr, BG_DARK, 1 );
  cairo_select_font_face(cr, "impact", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  
  // draw header
  cairo_rectangle( cr, x_, y_, w_, 14 );
  theme_->color( cr, HIGHLIGHT, 1.0 );
  cairo_fill( cr );
  
  /// TODO: get the dialog text amount, wrap based on text extents
  /// and draw buttons in Y position to not interfere with text
  
  // show the header
  cairo_text_extents_t extents;
  cairo_set_font_size(cr, 10.0);
  cairo_text_extents(cr, label(), &extents);
  cairo_move_to(cr, x_ + 4, y_ + 7 + (extents.height / 2) );
  theme_->color( cr, BG_DARK, 1 );
  cairo_show_text( cr, label() );
  
  // show the content
  cairo_set_font_size(cr, 12.0);
  cairo_text_extents(cr, contents.c_str(), &extents);
  cairo_move_to(cr, x_ + 4, y_  + 25 + 7 + (extents.height / 2) );
  //theme_->color( cr, BG_DARK, 1 );
  cairo_set_source_rgb( cr, 1,1,1 );
  cairo_show_text( cr, contents.c_str() );
  
  // buttons_
  if( buttons_ == OK )
  {
    ok->x( x_ + w_ * 0.8  );
    ok->y( y_ + h_ * 0.75 );
    ok->visible( true );
  }
  else if( buttons_ == OK_CANCEL )
  {
    ok->x( x_ + w_ * 0.8 - 70  );
    ok->y( y_ + h_ * 0.75 );
    cancel->x( x_ + w_ * 0.8  );
    cancel->y( y_ + h_ * 0.75 );
    ok->visible( true );
    cancel->visible( true );
  }
  
  Group::draw( cr );
  
  cairo_restore( cr );
}

// to highjack the OK / Cancel buttons events
void Dialog::valueCB( Avtk::Widget* widget)
{
  if( widget == ok )
  {
    returnVal = 1;
    visible( false );
  }
  else if( widget == cancel )
  {
    returnVal = 0;
    visible( false );
  }
  else
  {
    //printf("Dialog::widgetValueCB() widget %s\n", widget->label() );
  }
}

int Dialog::run( const char* text, BUTTONS b )
{
  // show the dialog
  buttons_ = b;
  contents = text;
  visible( true );
  
  ok->value( false );
  cancel->value( false );
  
  // make UI route all events to here, and stall
  ui->handleOnly( this );
  
  returnVal = -2;
  while( returnVal == -2 )
  {
    ui->idle();
    usleep(10000);
  }
  
  ui->handleOnly( 0x0 );
  
  return returnVal;
}

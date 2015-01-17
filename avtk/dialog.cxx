
#include "dialog.hxx"

#include "button.hxx"
#include "ui.hxx"
#include "theme.hxx"

#include <stdio.h>

using namespace Avtk;

Dialog::Dialog( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ )
{
  clickMode( CLICK_MOMENTARY );
  
  ok     = new Button( ui, 0, 0, 60, 20, "OK" );
  cancel = new Button( ui, 0, 0, 60, 20, "Cancel" );
  ok    ->visible( false );
  cancel->visible( false );
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
  
  // draw dark BG
  cairo_rectangle( cr, x_, y_, w_, h_ );
  theme_->color( cr, BG_DARK, 1 );
  cairo_fill_preserve( cr );
  cairo_stroke( cr );
  theme_->color( cr, BG_DARK, 0.8 );
  cairo_select_font_face(cr, "impact", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  
  // draw header
  cairo_rectangle( cr, x_, y_, w_, 14 );
  theme_->color( cr, HIGHLIGHT, 0.8 );
  cairo_fill( cr );
  
  /// TODO: get the dialog text amount, wrap based on text extents
  /// and draw buttons in Y position to not interfere with text
  
  // show the text
  cairo_text_extents_t extents;
  cairo_set_font_size(cr, 10.0);
  cairo_text_extents(cr, label(), &extents);
  cairo_move_to(cr, x_ + 4, y_ + 7 + (extents.height / 2) );
  theme_->color( cr, BG_DARK, 1 );
  cairo_show_text( cr, label() );
  
  // buttons_
  if( buttons_ == OK )
  {
    ok->x( x_ + w_ * 0.8  );
    ok->y( y_ + h_ * 0.75 );
  }
  if( buttons_ == OK_CANCEL )
  {
    ok->x( x_ + w_ * 0.8 - 70  );
    ok->y( y_ + h_ * 0.75 );
    cancel->x( x_ + w_ * 0.8  );
    cancel->y( y_ + h_ * 0.75 );
  }
  
  cairo_restore( cr );
}

void Dialog::show( const char* text, BUTTONS b )
{
  ok    ->visible( true );
  cancel->visible( true );
  buttons_ = b;
  contents = text;
  visible( true );
}

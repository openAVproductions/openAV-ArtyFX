
#include "list.hxx"

#include "ui.hxx"
#include "theme.hxx"

#include <stdio.h>

using namespace Avtk;

List::List( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ )
{
  // set the click mode -> value() to Y position, with every 11 px becoming the
  // next item. the Widget parent class takes care of the details.
  clickMode( CLICK_VALUE_FROM_Y );
  
  items.push_back("One");
  items.push_back("Two");
  items.push_back("Three");
  items.push_back("Four");
  items.push_back("Five");
  items.push_back("Six");
  items.push_back("Seven");
  items.push_back("Eight");
  items.push_back("Nine");
  items.push_back("Ten");
  
  scrollInvert = true;
}

void List::show( std::vector< std::string > data )
{
  items = data;
  ui->redraw( this );
}

int List::selectItem()
{
  return items.size() * value();
}

void List::selectItem( int select )
{
  value( select / value() );
}

void List::draw( cairo_t* cr )
{
  cairo_save( cr );
  
  roundedBox(cr, x, y, w, h, theme_->cornerRadius_ );
  
  int selectedItem = value() * (items.size()-1);
  
  roundedBox(cr, x, y, w, h, 1 );
  theme_->color( cr, HIGHLIGHT, 0.4 );
  cairo_fill_preserve(cr);
  theme_->color( cr, HIGHLIGHT );
  cairo_set_line_width(cr, 1.4);
  cairo_stroke(cr);
  
  // Draw items in the list
  for(int i = 0; i < items.size(); i++ )
  {
    cairo_text_extents_t extents;
    cairo_set_font_size(cr, 10.0);
    cairo_text_extents(cr, label(), &extents);
    
    int iY = y + 10 + extents.height / 2 - 2 + 12 * i;
    
    if( i == selectedItem )
    {
      cairo_rectangle( cr, x, iY - 9, w, 11 );
      theme_->color( cr, FG, 0.4 );
      cairo_fill_preserve( cr );
      theme_->color( cr, HIGHLIGHT, 0.8 );
      cairo_stroke( cr );
      //cairo_set_source_rgb( cr, 1,1,1 );
      theme_->color( cr, BG_DARK, 0.8 );
    }
    else
    {
      theme_->color( cr, BG_DARK );
    }
    
    cairo_move_to(cr, x + 5, iY );
    cairo_show_text( cr, items.at(i).c_str() );
  }
  
  
  cairo_restore( cr );
}


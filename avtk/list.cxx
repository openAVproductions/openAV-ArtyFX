
#include "list.hxx"

#include "ui.hxx"
#include "theme.hxx"
#include "listitem.hxx"

#include <stdio.h>

using namespace Avtk;

List::List( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Group( ui, x_, y_, w_, h_, label_ )
{
  mode( Group::WIDTH_EQUAL );
  lastClickedItem = -1;
}

void List::addItem( std::string newItem )
{
  items.push_back( newItem );
  
  add( new Avtk::ListItem( ui, 0, 0, 11, 11, newItem ) );
  
  ui->redraw();
}

void List::show( std::vector< std::string > data )
{
  items.swap( data );
  
  for(int i = 0; i < items.size(); i++ )
  {
    add( new Avtk::ListItem( ui, 0, 0, 11, 11, items.at(i) ) );
  }
  
  ui->redraw();
}

void List::clear()
{
  // free the widgets
  Group::clear();
  // invalidate last item
  lastClickedItem = -1;
}

void List::draw( cairo_t* cr )
{
  cairo_save( cr );
  
  // have the group draw itself
  Group::draw( cr );
  
  roundedBox(cr, x, y, w, h, theme_->cornerRadius_ );
  theme_->color( cr, HIGHLIGHT );
  cairo_stroke( cr );
  
  cairo_restore( cr );
}

std::string List::selectedString()
{
  if( lastClickedItem == -1 )
  {
    return "";
  }
  return items.at( lastClickedItem );
}

void List::valueCB( Widget* w )
{
  // call the super valueCB, handles turning off other widgets
  Group::valueCB( w );
  lastClickedItem = w->groupItemNumber();
  printf("list: lastClickedItem# %i, string: %s\n", lastClickedItem, selectedString().c_str() );
  
  // send an event to UI as the list widget
  Avtk::UI::staticWidgetValueCB( this, ui );
}

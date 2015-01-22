
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
  valueMode( Group::VALUE_SINGLE_CHILD );
  lastClickedItem = -1;
}

void List::addItem( std::string newItem )
{
  items.push_back( newItem );
  add( new Avtk::ListItem( ui, 0, 0, 34, 14, newItem ) );
}

void List::show( std::vector< std::string > data )
{
  items.swap( data );
  
  for(int i = 0; i < items.size(); i++ )
  {
    add( new Avtk::ListItem( ui, 0, 0, 11, 11, items.at(i) ) );
  }
}

void List::value( float v )
{
  int item = int(v);
  for( int i = 0; i < children.size(); i++ )
  {
    bool v = false;
    if( item == i )
      v = true;
    
    children.at(i)->value( v );
  }
  lastClickedItem = item;
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
  if( Widget::visible() )
  {
    cairo_save( cr );
    
    Group::draw( cr );
    
    cairo_restore( cr );
  }
}

std::string List::selectedString()
{
  if( lastClickedItem == -1 ||
      lastClickedItem >= items.size() )
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
  
  // copy mouse event co-ords to List widget
  mouseButtonPressed_ = w->mouseButton();
  mousePressX = w->mouseX();
  mousePressY = w->mouseY();
  
  std::string tmp = selectedString();
  if( !tmp.size() )
  {
    return;
  }
  
  printf("list: lastClickedItem# %i, string: %s\n", lastClickedItem, selectedString().c_str() );
  
  // send an event to UI as the list widget
  if( callback )
    callback( this, callbackUD );
  
  //Avtk::UI::staticWidgetValueCB( this, ui );
}

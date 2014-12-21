
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
  printf("list: lastClickedItem# %i\n", lastClickedItem );
}

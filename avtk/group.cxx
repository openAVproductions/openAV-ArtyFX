
#include "group.hxx"

#include "ui.hxx"
#include "avtk.hxx"

namespace Avtk
{


Group::Group( Avtk::UI* ui, int x, int y, int w, int h, std::string label ) :
  Widget( ui, x, y, w, h, label ),
  groupMode( NONE )
{
  noHandle_ = true;
}

void Group::add( Widget* child )
{
  printf("Group add: size %i\n", children.size() );
  child->addToGroup( this, children.size() );
  
  // capture callback of the child widget
  child->callback   = staticGroupCB;
  child->callbackUD = this;
  
  // set the child's co-ords
  const int border = 0;
  
  if( groupMode == WIDTH_EQUAL )
  {
    child->x = x;
    child->w = w;
    
    int childY = y;
    for(int i = 0; i < children.size(); i++ )
      childY += children.at(i)->h + border;

    child->y = childY;
  }
  else if( groupMode == HEIGHT_EQUAL )
  {
    child->y = y;
    child->h = h;
    
    int childX = x;
    for(int i = 0; i < children.size(); i++ )
      childX += children.at(i)->w + border;
    child->x = childX;
  }
  
  children.push_back( child );
  
  printf("Group after add: size %i\n", children.size() );
  
  ui->redraw();
}

void Group::clear()
{
  for(int i = 0; i < children.size(); i++ )
  {
    delete children.at(i);
  }
  // resets size of vector to 0
  children.clear();
}

void Group::mode( GROUP_MODE gm )
{
  groupMode = gm;
}

void Group::valueCB( Widget* w )
{
  printf("Group child # %i : value : %f\tNow into Normal CB\n", w->groupItemNumber(), w->value() );
  
  for(int i = 0; i < children.size(); i++ )
  {
    children.at(i)->value( false );
  }
  
  w->value( true );
  
  // optionally continue to normal callback
  if( false )
  {
    Avtk::UI::staticWidgetValueCB( w, ui );
  }
}

void Group::draw( cairo_t* cr )
{
  if( visible() )
  {
    for(int i = 0; i < children.size(); i++ )
    {
      children.at( i )->draw( cr );
    }
  }
}

Group::~Group()
{
}

};

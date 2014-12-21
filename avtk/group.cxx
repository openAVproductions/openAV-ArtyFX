
#include "group.hxx"

#include "ui.hxx"
#include "avtk.hxx"

namespace Avtk
{


Group::Group( Avtk::UI* ui, int x, int y, int w, int h, std::string label ) :
  Widget( ui, x, y, w, h, label ),
  groupMode( GROUP_WIDTH_EQUAL )
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
  int childY = y;
  for(int i = 0; i < children.size(); i++ )
  {
    childY += children.at(i)->h + border;
  }
  
  child->x = x;
  child->y = childY;
  
  if( groupMode == GROUP_WIDTH_EQUAL )
  {
    child->w = w;
  }
  if( groupMode == GROUP_HEIGHT_EQUAL )
  {
    child->h = h;
  }
  
  children.push_back( child );
  
  printf("Group after add: size %i\n", children.size() );
  
  ui->redraw();
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

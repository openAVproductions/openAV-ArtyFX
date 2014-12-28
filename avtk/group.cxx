
#include "group.hxx"

#include "ui.hxx"
#include "avtk.hxx"

namespace Avtk
{


Group::Group( Avtk::UI* ui ) :
  Widget( ui ),
  groupMode( NONE )
{
  noHandle_ = true;
}

Group::Group( Avtk::UI* ui, int x, int y, int w, int h, std::string label ) :
  Widget( ui, x, y, w, h, label ),
  groupMode( NONE )
{
  noHandle_ = true;
}

void Group::add( Widget* child )
{
  // if widget is currently already parented (not a new widget), remove it from
  // its previous parent group.
  if( child->parent() )
  {
    child->parent()->remove( child );
  }
  
#ifdef AVTK_DEBUG
  printf("Group add: size %i\n", children.size() );
#endif
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

#ifdef AVTK_DEBUG
  printf("Group after add: size %i\n", children.size() );
#endif
  
  ui->redraw();
}

void Group::remove( Avtk::Widget* w )
{
  for(int i = 0; i < children.size(); i++ )
  {
    if( children.at(i) == w )
    {
      printf("Group::remove() %s, widget# %i\n", label(), i );
      children.erase( children.begin() + i );
    }
  }
}

void Group::visible( bool vis )
{
  Widget::visible( vis );
  for(int i = 0; i < children.size(); i++ )
  {
    children.at(i)->visible( vis );
  }
}

bool Group::visible()
{
  /*
  for(int i = 0; i < children.size(); i++ )
  {
    if( !children.at(i)->visible() )
      return false;
  }
  */
  return Widget::visible();
}

void Group::clear()
{
  while( children.size() > 0 )
  {
#ifdef AVTK_DEBUG
    printf("removing child %s from UI\n", children.at(0)->label() );
#endif
    ui->remove( children.at(0) );
    Avtk::Widget* tmp = children.at(0);
    children.erase( children.begin() );
    delete tmp;
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
  // only one widget is value( true ) in a group at a time
  if( false )
  {
#ifdef AVTK_DEBUG
    printf("Group child # %i : value : %f\tNow into Normal CB\n", w->groupItemNumber(), w->value() );
#endif
    for(int i = 0; i < children.size(); i++ )
    {
      children.at(i)->value( false );
    }
    
    w->value( true );
  }
  
  // continue to widget's callback
  if( true )
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

int Group::handle( const PuglEvent* event )
{
  if( visible() )
  {
    // TODO: reverse iter here?
    for(int i = 0; i < children.size(); i++ )
    {
      int ret = children.at( i )->handle( event );
      if( ret )
      {
        //printf("widget %i handle eventType %i ret\n", i, event->type );
        return ret; // child widget ate event: done :)
      }
    }
  }
  return 0;
}

Group::~Group()
{
  // on deletion, clean up all widgets left as children
  clear();
}

};

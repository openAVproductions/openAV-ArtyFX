
#include "group.hxx"

#include "ui.hxx"
#include "avtk.hxx"

namespace Avtk
{


Group::Group( Avtk::UI* ui ) :
  Widget( ui ),
  groupMode( NONE ),
  valueMode_( VALUE_NORMAL ),
  spacing_( 0 )
{
  noHandle_ = true;
}

Group::Group( Avtk::UI* ui, int x, int y, int w, int h, std::string label ) :
  Widget( ui, x, y, w, h, label ),
  groupMode( NONE ),
  valueMode_( VALUE_NORMAL ),
  spacing_( 1 )
{
  noHandle_ = false;
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
  if( groupMode == WIDTH_EQUAL )
  {
    child->x( x_ );
    child->w( w_ );
    
    int childY = y_;
    for(int i = 0; i < children.size(); i++ )
      childY += children.at(i)->h() + spacing_;

    child->y( childY );
  }
  else if( groupMode == HEIGHT_EQUAL )
  {
    child->y( y_ );
    child->h( h_ );
    
    int childX = x_;
    for(int i = 0; i < children.size(); i++ )
      childX += children.at(i)->w() + spacing_;
    child->x( childX );
  }
  
  children.push_back( child );

#ifdef AVTK_DEBUG
  printf("Group after add: size %i\n", children.size() );
#endif
  
  ui->redraw();
}

void Group::remove( Avtk::Widget* wid )
{
  for(int i = 0; i < children.size(); i++ )
  {
    if( children.at(i) == wid )
    {
#ifdef AVTK_DEBUG
      printf("Group::remove() %s, widget# %i\n", label(), i );
#endif
      children.erase( children.begin() + i );
    }
  }
}

void Group::resizeNotify( Widget* w )
{
#ifdef AVTK_DEBUG
      printf("Group::resizeNotify() %s, widget %s\n", label(), w->label() );
#endif
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
  if( valueMode_ == VALUE_SINGLE_CHILD )
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

void Group::x(int x__)
{
  int d = x__ - x_;
  x_ = x__;
  for(int i = 0; i < children.size(); i++ )
  {
    Widget* c = children.at(i);
    c->x( c->x() + d );
  }
}

void Group::y(int y__)
{
  int d = y__ - y_;
  y_ = y__;
  for(int i = 0; i < children.size(); i++ )
  {
    Widget* c = children.at(i);
    c->y( c->y() + d );
  }
}

void Group::w(int w__)
{
  int d = w__ - w_;
  w_ = w__;
  if( groupMode == WIDTH_EQUAL )
  {
    for(int i = 0; i < children.size(); i++ )
    {
      Widget* c = children.at(i);
      c->w( c->w() + d );
    }
  }
}

void Group::h(int h__)
{
  int d = h__ - h_;
  h_ = h__;
  if( groupMode == HEIGHT_EQUAL )
  {
    for(int i = 0; i < children.size(); i++ )
    {
      Widget* c = children.at(i);
      c->h( c->h() + d );
    }
  }
}

void Group::draw( cairo_t* cr )
{
  if( visible() )
  {
    for(int i = 0; i < children.size(); i++ )
    {
      Widget* c = children.at( i );
      if( c->visible() )
        c->draw( cr );
    }
    
    roundedBox(cr, x_, y_, w_, h_, theme_->cornerRadius_ );
    theme_->color( cr, FG );
    cairo_set_line_width(cr, 0.5);
    cairo_stroke(cr);
    
    //cairo_surface_write_to_png( cairo_get_target( cr ), "cr.png" );
  }
}

int Group::handle( const PuglEvent* event )
{
  if( visible() )
  {
    // reverse iter over the children: top first
    for(int i = children.size() - 1; i >= 0 ; i-- )
    {
      int ret = children.at( i )->handle( event );
      if( ret )
      {
        //printf("widget %i handle eventType %i ret\n", i, event->type );
        return ret; // child widget ate event: done :)
      }
    }
    
    // if we haven't returned, the event was not consumed by the children, so we
    // can check for a scroll event, and if yes, highlight the next item
    if( event->type == PUGL_SCROLL && 
        valueMode_ == VALUE_SINGLE_CHILD && 
        touches( event->scroll.x, event->scroll.y ) &&
        children.size() > 0 )
    {
      // find value() widget
      int vw = -1;
      for(int i = children.size() - 1; i >= 0 ; i-- )
      {
        if( children.at(i)->value() > 0.4999 )
          vw = i;
      }
      
      int d = event->scroll.dy;
      //printf("SCROLL: Value child %i, delta %i\n", vw, d );
      
      // no widget selected
      if( vw == -1 )
      {
        children.at(0)->value( true );
      }
      // scroll up
      else if( vw > 0 && d > 0 )
      {
        children.at(vw-1)->value( true  );
        children.at(vw  )->value( false );
      }
      // scroll down
      else if( vw < children.size()-1 && d < 0 )
      {
        children.at(vw  )->value( false );
        children.at(vw+1)->value( true  );
      }
      
      // handled scroll, so eat event
      return 1;
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

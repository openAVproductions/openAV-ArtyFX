
#include "ui.hxx"

#include "avtk.hxx"
#include "theme.hxx"

using namespace Avtk;

UI::UI( int w__, int h__, PuglNativeWindow parent ) :
  quit_( false ),
  w_( w__ ),
  h_( h__ )
{
  view = puglInit(NULL, NULL);
  
  if( parent != 0 )
    puglInitWindowParent( view, parent );
  
  puglInitWindowSize  (view, w_, h_ );
  puglInitResizable   (view, false );
  puglInitContextType (view, PUGL_CAIRO);
  /*
  puglIgnoreKeyRepeat (view, ignoreKeyRepeat);
  */
  puglSetEventFunc    (view, UI::onEvent  );
  puglSetDisplayFunc  (view, UI::onDisplay);
  puglSetCloseFunc    (view, UI::onClose  );
  puglSetMotionFunc   (view, UI::onMotion );
  
  puglCreateWindow    (view, "Avtk");
  puglShowWindow      (view);
  
  puglSetHandle       (view, this);
  
  //themes.push_back( new Theme( this ) );
  
  motionUpdateWidget = 0;
  
  dragDropOrigin   = 0;
  dragDropDataSize = 0;
  dragDropDataPtr  = 0;
  
  dragDropTargetVerified       = false;
  dragDropTargetVerifiedWidget = 0;
  
  themes.push_back( new Theme( this, "default.avtk" ) );
}

void UI::display( cairo_t* cr )
{
  /// clear the screen
  cairo_rectangle( cr, 0, 0, w_, h_ );
  cairo_set_source_rgb( cr, 24/255., 24/255., 24/255. );
  cairo_fill( cr );
  
  /// iter over widgets, drawing each in the order they were add()-ed
  int i = 0;
  for (std::list<Avtk::Widget*>::iterator it = widgets.begin(); it != widgets.end(); it++)
  {
    //printf("display() widget # %i\n", i++ );
    if( (*it)->visible() )
      (*it)->draw( cr );
  }
}

Theme* UI::theme( int id )
{
  if( id < themes.size() )
    return themes.at( id );
  
  // default theme
  return themes.at( 0 );
}

void UI::event( const PuglEvent* event )
{
  // reverse iter over widgets (aka starting on "top"), calling handle()
  for (std::list< Avtk::Widget*>::iterator it = widgets.begin(); it != widgets.end(); it++ )
  {
    if( (*it)->visible() )
      if( (*it)->handle( event ) )
        return;
  }
  
  // code is only reached if *none* of the widgets handled an event:
  // we can implement UI wide hotkeys here, handle unknown events
  switch (event->type)
  {
    case PUGL_BUTTON_PRESS:
      break;
    
    case PUGL_KEY_PRESS:
      if (event->key.character == 'q' ||
          event->key.character == 'Q' ||
          event->key.character == PUGL_CHAR_ESCAPE)
      {
        quit_ = 1;
      }
      break;
    
    case PUGL_EXPOSE:
      //printf("recieved pugl expose in UI\n");
      //redraw();
      break;
    
    default:
      break;
  }
}

void UI::redraw()
{
  puglPostRedisplay( view );
}

void UI::redraw( Avtk::Widget* w )
{
  puglPostExpose( view, w->x, w->y, w->w, w->h );
}

void UI::motion(int x, int y)
{
  if( motionUpdateWidget )
  {
    motionUpdateWidget->motion( x, y );
  }
  else if( dragDropOrigin )
  {
    // scan trough widgets on mouse-move, as it *could* be a drag-drop action.
    for (std::list< Avtk::Widget* >::iterator it = widgets.begin(); it != widgets.end(); it++)
    {
      if( (*it)->touches( x, y ) )
      {
        //printf("DragDropVerify: Origin %s, Target %s\n", dragDropOrigin->label(), (*it)->label() );
        dragDropVerify( (*it) );
      }
    }
  }
}

void UI::dragDropInit( Avtk::Widget* origin, size_t size, void* data )
{
  // set the dragDropOrigin widget, and set the motionUpdateWidget to NULL.
  dragDropOrigin = origin;
  
  motionUpdateWidget = 0;
  
  if( dragDropDataPtr )
  {
#ifdef AVTK_DEBUG
    printf("UI delete[] existing dragDropDataPtr\n");
#endif // AVTK_DEBUG
    delete[] dragDropDataPtr;
  }
  
#ifdef AVTK_DEBUG
  printf("UI new dragDropDataPtr, size %i\n", size);
#endif // AVTK_DEBUG
  dragDropDataSize = size;
  dragDropDataPtr  = new char[size];
  
  memcpy( dragDropDataPtr, data, size );
}

bool UI::dragDropVerify( Avtk::Widget* target )
{
  if ( dragDropTargetVerified && dragDropTargetVerifiedWidget == target )
  {
    // we've already found a valid match for this widget, just return true
    return true;
  }
  else
  {
    // reset search for a match
    dragDropTargetVerifiedWidget = 0;
  }
  
  // haven't tested this widget yet
  if( dragDropTargetVerifiedWidget == 0 )
  {
    // TODO: match in the origin data-types and target data-types
    for( int i = 0; i < 1/*dragDropOrigin->dragDropDataTypes()*/; i++ )
    {
      if( true /*target->dragDropDataTypeCheck( i )*/ )
      {
        dragDropTargetVerified = true;
        dragDropTargetVerifiedWidget = target;
#ifdef AVTK_DEBUG
        printf("DragDropVerify to %s OK: data = %s\n", target->label(), dragDropDataPtr );
#endif // AVTK_DEBUG
        return true;
      }
    }
    
    dragDropTargetVerified = false;
#ifdef AVTK_DEBUG
    printf("DragDropVerify Failed no data-type matches\n" );
#endif // AVTK_DEBUG
    return false;
  }
  
  return false;
}

void UI::dragDropComplete( Avtk::Widget* target )
{
  
}

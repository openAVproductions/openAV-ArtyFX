
#include "ui.hxx"

#include "avtk.hxx"
#include "theme.hxx"

#ifdef AVTK_TESTER
#include "tester.hxx"
#endif

using namespace Avtk;

UI::UI( int w__, int h__, PuglNativeWindow parent, const char* windowName ) :
  Group( this, w__, h__ ),
  quit_( false ),
  w_( w__ ),
  h_( h__ )
#ifdef AVTK_TESTER
  , tester( new Tester( this ) ) // nasty but necessary
#endif
{
  view = puglInit(NULL, NULL);
  
  parentStack.push( this );
  
  if( parent != 0 )
    puglInitWindowParent( view, parent );
  
  puglInitWindowSize  (view, w_, h_ );
  puglInitResizable   (view, true );
  puglInitContextType (view, PUGL_CAIRO);
  puglIgnoreKeyRepeat (view, true );
  
  puglSetEventFunc    (view, UI::onEvent  );
  puglSetDisplayFunc  (view, UI::onDisplay);
  puglSetCloseFunc    (view, UI::onClose  );
  puglSetMotionFunc   (view, UI::onMotion );
  puglSetReshapeFunc  (view, UI::onReshape);
  
  puglCreateWindow    (view, windowName );
  puglShowWindow      (view);
  
  puglSetHandle       (view, this);
  
  //themes.push_back( new Theme( this ) );
  
  motionUpdateWidget = 0;
  handleOnlyWidget = 0;
  
  dragDropOrigin   = 0;
  dragDropDataSize = 0;
  dragDropDataPtr  = 0;
  
  dragDropTargetVerified       = false;
  dragDropTargetVerifiedWidget = 0;
  
  themes.push_back( new Theme( this, "default.avtk" ) );
}

void UI::reshape(int x, int y)
{
#ifdef AVTK_DEBUG_DTOR
  printf("reshaping UI: scale factor: %f \t%f\n", x/float(initW), y/float(initH) );
#endif
  
  //Group::resize( );
  
  //Group::w( x );
  //Group::h( y );
}

UI::~UI()
{
#ifdef AVTK_DEBUG_DTOR
  printf("%s %s\n", __PRETTY_FUNCTION__, label() );
#endif
  
  while( themes.size() > 0 )
  {
    Theme* tmp = themes.at(0);
    themes.erase( themes.begin() );
    delete tmp;
  }
  
  puglDestroy( view );
}

void UI::display( cairo_t* cr )
{
  /// clear the screen
  cairo_rectangle( cr, 0, 0, w_, h_ );
  cairo_set_source_rgb( cr, 24/255., 24/255., 24/255. );
  cairo_fill( cr );
  
  /// use the group abstraction to draw the widgets, as the UI class derives
  /// from a Group, this process is simple.
  Group::draw( cr );
}

Theme* UI::theme( int id )
{
  if( id < themes.size() )
    return themes.at( id );
  
  // default theme
  return themes.at( 0 );
}

int UI::idle()
{
  puglProcessEvents(view);
  return quit_;
}

int UI::run()
{
  redraw();
  
  while ( !quit_ )
  {
    puglProcessEvents(view);
    usleep( 25000 );
    
#ifdef AVTK_TESTER
    tester->process();
#endif
    
  }
  
  return 0;
}

void UI::pushParent( Avtk::Group* g )
{
#ifdef AVTK_DEBUG
  printf("%s - pushing %s to stack\n", __PRETTY_FUNCTION__, g->label() );
#endif
  parentStack.push( g );
}

void UI::popParent()
{
#ifdef AVTK_DEBUG
  printf("%s - popping %s from stack\n", __PRETTY_FUNCTION__, parentStack.top()->label() );
#endif
  parentStack.pop();
}

void UI::remove( Avtk::Widget* w )
{
  if( w == handleOnlyWidget )
  {
    handleOnlyWidget = 0x0;
  }
  else if ( w == motionUpdateWidget )
  {
    motionUpdateWidget = 0x0;
  }
  else if ( w == dragDropOrigin )
  {
    dragDropOrigin = 0x0;
  }
  else if ( w == dragDropTargetVerifiedWidget )
  {
    dragDropTargetVerifiedWidget = 0x0;
  }
  
  Group::remove( w );
}

void UI::handleOnly( Widget* wid )
{
  handleOnlyWidget = wid;
}

void UI::event( const PuglEvent* event )
{
  if( event->type != PUGL_EXPOSE )
  {
    //printf("UI::handle() type = %i, sending to Tester\n", event->type );
#ifdef AVTK_TESTER
    // eat AVTK start record events shortcut: 
    if( event->type == PUGL_KEY_PRESS )
    {
      // ^1 pressed (Ctrl and number 1)
      if( event->key.character == 'a' )
        //&& (((PuglEventKey*)event)->state & PUGL_MOD_CTRL) )
      {
        if( !tester->recording() )
        {
          printf("AVTK: Tester Record Starting!\n");
          tester->record( "test1" );
        }
        else
        {
          tester->recordStop();
        }
        return;
      }
      
      // replay on Ctrl^2
      if( event->key.character == 's' )
        // && (((PuglEventKey*)event)->state & PUGL_MOD_CTRL) )
      {
        printf("run test!\n");
        tester->runTest( "test1" );
        return;
      }
    }
    else
    {
      tester->handle( event );
    }
#endif
    
    if( handleOnlyWidget )
    {
      handleOnlyWidget->handle( event );
      internalEvent( event );
      return;
    }
    
  
    // pass event to group to be handled
    int ret = Group::handle( event );
    if ( ret )
    {
      redraw();
      return;
    }
    
    // if no widget handles the event, then we test the main UI shortcuts
    internalEvent( event );
    
  }
  else if( event->type == PUGL_CONFIGURE )
  {
#ifdef AVTK_DEBUG
    printf("UI handleing PUGL_CONFIGURE\n");
#endif
  }
}

void UI::internalEvent( const PuglEvent* event )
{
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
        if( handleOnlyWidget )
        {
          handleOnlyWidget->visible( false );
          handleOnlyWidget = 0x0;
          redraw();
        }
        else
        {
          quit_ = 1;
        }
      }
      break;
    
    default:
      break;
  }
  
  return;
}

void UI::redraw()
{
  puglPostRedisplay( view );
}

void UI::redraw( Avtk::Widget* w )
{
  puglPostRedisplay( view );
  //puglPostExpose( view, w->x, w->y, w->w, w->h );
}

void UI::motion(int x, int y)
{
  if( motionUpdateWidget )
  {
    motionUpdateWidget->motion( x, y );
  }
  else if( dragDropOrigin )
  {
    /*
    refactor to use GROUP!
    // scan trough widgets on mouse-move, as it *could* be a drag-drop action.
    for (std::list< Avtk::Widget* >::iterator it = widgets.begin(); it != widgets.end(); it++)
    {
      if( (*it)->touches( x, y ) )
      {
        //printf("DragDropVerify: Origin %s, Target %s\n", dragDropOrigin->label(), (*it)->label() );
        dragDropVerify( (*it) );
      }
    }
    */
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

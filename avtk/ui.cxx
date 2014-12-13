
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
  
  puglSetSpecialFunc  (view, UI::onSpecial);
  
  puglCreateWindow    (view, "Avtk");
  puglShowWindow      (view);
  
  puglSetHandle       (view, this);
  
  theme = new Theme( this );
  
  motionUpdateWidget = 0;
}

void UI::display( cairo_t* cr )
{
  /// clear the screen
  cairo_rectangle( cr, 0, 0, w_, h_ );
  cairo_set_source_rgb( cr, 24/255., 24/255., 24/255. );
  cairo_fill( cr );
  
  /// iter over widgets, drawing each in the order they were add()-ed
  int i = 0;
  for (std::list< ptr<Avtk::Widget> >::iterator it = widgets.begin(); it != widgets.end(); it++)
  {
    //printf("display() widget # %i\n", i++ );
    (*it)->draw( cr );
  }
}

void UI::event( const PuglEvent* event )
{
  // reverse iter over widgets (aka starting on "top"), calling handle()
  for (std::list< ptr<Avtk::Widget> >::iterator it = widgets.begin(); it != widgets.end(); it++ )
  {
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
      printf("recieved pugl expose in UI\n");
      redraw();
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
    motionUpdateWidget->drag( x, y );
  }
}


#include "ui.hxx"

#include "avtk.hxx"

using namespace Avtk;

UI::UI( int w__, int h__ ) :
  quit_( false ),
  w_( w__ ),
  h_( h__ )
{
  view = puglInit(NULL, NULL);
  
  puglInitWindowSize  (view, w_, h_ );
  puglInitResizable   (view, false );
  puglInitContextType (view, PUGL_CAIRO);
  /*
  puglIgnoreKeyRepeat (view, ignoreKeyRepeat);
  */
  puglSetEventFunc    (view, UI::onEvent  );
  puglSetDisplayFunc  (view, UI::onDisplay);
  puglSetCloseFunc    (view, UI::onClose  );
  
  puglCreateWindow    (view, "Vizia GUI");
  puglShowWindow      (view);
  
  puglSetHandle       (view, this);
}

void UI::display( cairo_t* cr )
{
  /// clear the screen
  cairo_rectangle( cr, 0, 0, w_, h_ );
  cairo_set_source_rgb( cr, 24/255, 24/255, 24/255 );
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
  switch (event->type)
  {
    case PUGL_KEY_PRESS:
      if (event->key.character == 'q' ||
          event->key.character == 'Q' ||
          event->key.character == PUGL_CHAR_ESCAPE)
      {
        quit_ = 1;
      }
      break;
    
    case PUGL_BUTTON_PRESS: {
      // iter widgets, checking for intersection
      int i = 0;
      for (std::list< ptr<Avtk::Widget> >::iterator it = widgets.begin(); it != widgets.end(); it++)
      {
        //printf("EVENT() widget # %i\n", i++ );
        if( (*it)->touches( event->button.x, event->button.y, true ) )
        {
          (*it)->value( !(*it)->value() );
          printf("touches widget # %i, new value %f\n", i, (*it)->value() );
        }
        i++;
      }
      
      puglPostRedisplay(view); }
      break;
    
    default:
      break;
  }
}

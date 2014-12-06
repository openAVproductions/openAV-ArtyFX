
#ifndef OPENAV_AVTK_UI_HXX
#define OPENAV_AVTK_UI_HXX

#include "avtk.hxx"

#include <list>
#include <stdio.h>
#include <unistd.h>

using namespace yasper;

namespace Avtk
{

class Avtk::Widget;

class UI
{
  public:
    UI( int w__, int h__ ) :
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
    
    /// adds a widget to the UI, and memory manages it: AKA a smart pointer will
    /// clean up on close of the UI.
    void add( Avtk::Widget* w)
    {
      widgets.push_back( w );
    }
    
    int run( int fps = 30)
    {
      int delay = (1.f / fps) * 1000 * 1000;
      printf("UI running at %i fps, delay %i", fps, delay );
      
      while ( !quit_ )
      {
        puglProcessEvents(view);
        usleep( delay );
      }
      
      return 0;
    }
    
    ~UI()
    {
      puglDestroy( view );
    }
  
  private:
    PuglView* view;
    
    bool quit_;
    int w_, h_;
    
    std::list< ptr<Avtk::Widget> > widgets;
    
    static void widgetCB(Avtk::Widget* w, void* ud)
    {
      printf( "widgetCB(), label = %s\n", w->label.c_str() );
    }
    
    static void onClose(PuglView* view)
    {
      UI* ui = (UI*)puglGetHandle( view );
      ui->close();
    }
    
    void close()
    {
      quit_ = true;
    }
    
    static void onEvent(PuglView* view, const PuglEvent* event)
    {
      UI* ui = (UI*)puglGetHandle( view );
      ui->event( event );
    }
    
    void event( const PuglEvent* event )
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
            printf("event() widget # %i\n", i++ );
            if( (*it)->touches( event->button.x, event->button.y ) )
            {
              (*it)->value( !(*it)->value() );
              printf("touches widget # %i, new value %f\n", i++, (*it)->value() );
            }
          }
          
          puglPostRedisplay(view); }
          break;
        
        default:
          break;
      }
    }
    
    static void onDisplay(PuglView* view)
    {
      UI* ui = (UI*)puglGetHandle( view );
      cairo_t* cr = (cairo_t*)puglGetContext(view);
      ui->display( cr );
    }
    
    void display( cairo_t* cr )
    {
      /// clear the screen
      cairo_rectangle( cr, 0, 0, w_, h_ );
      cairo_set_source_rgb( cr, 24/255, 24/255, 24/255 );
      cairo_fill( cr );
      
      /// iter over widgets, drawing each in the order they were add()-ed
      int i = 0;
      for (std::list< ptr<Avtk::Widget> >::iterator it = widgets.begin(); it != widgets.end(); it++)
      {
        printf("display() widget # %i\n", i++ );
        (*it)->draw( cr );
      }
    }
};

}; // namespace Avtk

#endif // OPENAV_AVTK_UI_HXX

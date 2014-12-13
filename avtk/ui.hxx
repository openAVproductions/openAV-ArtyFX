
#ifndef OPENAV_AVTK_UI_HXX
#define OPENAV_AVTK_UI_HXX

// libs AVTK needs
#include "yasper.hxx"
#include "pugl/pugl.h"
#include <cairo/cairo.h>

// general C++ includes
#include <list>
#include <stdio.h>
#include <unistd.h>

using namespace yasper;


namespace Avtk
{

class Theme;
class Widget;


class UI
{
  public:
    UI( int w, int h, PuglNativeWindow parent = 0 );
    
    /// adds a widget to the UI, and memory manages it: AKA a smart pointer will
    /// clean up on close of the UI.
    void add( Avtk::Widget* w)
    {
      //printf("UI::add() pushing back w %i\n", w );
      widgets.push_back( w );
    }
    
    /// setDragWidget tells the UI a widget has captured a mouse-down event, and
    /// wants to be notified of mouse movement events
    void wantsMotionUpdates( Avtk::Widget* w, bool notifyOfMotion )
    {
      if( notifyOfMotion )
        motionUpdateWidget = w;
      else
        motionUpdateWidget = 0;
    }
    
    void redraw();
    void redraw( Avtk::Widget* w );
    
    int idle()
    {
      puglProcessEvents(view);
    }
    
    virtual int run()
    {
      redraw();
      
      while ( !quit_ )
      {
        puglProcessEvents(view);
        usleep( 10 );
      }
      
      return 0;
    }
    
    ~UI()
    {
      puglDestroy( view );
    }
    
    Theme* theme;
    
    /// call this to recieve the LV2 widget handle
    PuglNativeWindow getNativeHandle()
    {
      // returns the X11 handle, or Win32 surface, or Quartz surface
      return puglGetNativeWindow( view );
    }
  
  protected:
    PuglView* view;
    
    bool quit_;
    int w_, h_;
    
    /// the list of widgets currently instantiated, in order of being drawn.
    // Technically this is a list of yasper::ptr<Avtk::Widget> types, but they
    // act generally like raw pointers would do
    std::list< ptr<Avtk::Widget> > widgets;
    
    Avtk::Widget* motionUpdateWidget;
    
    
    void scroll( int x, int y, int dx, int dy );
    static void onScroll(PuglView* view, int x, int y, float dx, float dy)
    {
      UI* ui = (UI*)puglGetHandle( view );
      ui->scroll( x, y, dx, dy );
    }
    
    static void onSpecial(PuglView* view, bool pressed, PuglKey key)
    {
    }
    
    void motion(int x, int y);
    static void onMotion(PuglView* view, int x, int y)
    {
      UI* ui = (UI*)puglGetHandle( view );
      ui->motion( x, y );
    }
    
    void close() { quit_ = true; }
    static void onClose(PuglView* view)
    {
      UI* ui = (UI*)puglGetHandle( view );
      ui->close();
    }
    
    
    void event( const PuglEvent* event );
    static void onEvent(PuglView* view, const PuglEvent* event)
    {
      UI* ui = (UI*)puglGetHandle( view );
      ui->event( event );
    }
    
    void display( cairo_t* cr );
    static void onDisplay(PuglView* view)
    {
      UI* ui = (UI*)puglGetHandle( view );
      cairo_t* cr = (cairo_t*)puglGetContext(view);
      ui->display( cr );
    }
    
};

}; // namespace Avtk

#endif // OPENAV_AVTK_UI_HXX

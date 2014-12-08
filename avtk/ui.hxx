
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

class Widget;


class UI
{
  public:
    UI( int w, int h );
    
    /// adds a widget to the UI, and memory manages it: AKA a smart pointer will
    /// clean up on close of the UI.
    void add( Avtk::Widget* w)
    {
      //printf("UI::add() pushing back w %i\n", w );
      widgets.push_back( w );
    }
    
    void redraw()
    {
      puglPostRedisplay( view );
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
  
  private:
    PuglView* view;
    
    bool quit_;
    int w_, h_;
    
    /// the list of widgets currently instantiated, in order of being drawn.
    // Technically this is a list of yasper::ptr<Avtk::Widget> types, but they
    // act generally like raw pointers would do
    std::list< ptr<Avtk::Widget> > widgets;
    
    static void onSpecial(PuglView* view, bool pressed, PuglKey key)
    {
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
    
    void event( const PuglEvent* event );
    
    static void onDisplay(PuglView* view)
    {
      UI* ui = (UI*)puglGetHandle( view );
      cairo_t* cr = (cairo_t*)puglGetContext(view);
      ui->display( cr );
    }
    
    void display( cairo_t* cr );
};

}; // namespace Avtk

#endif // OPENAV_AVTK_UI_HXX

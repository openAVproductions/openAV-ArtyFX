
#ifndef OPENAV_AVTK_UI_HXX
#define OPENAV_AVTK_UI_HXX

// libs AVTK needs
#include "yasper.hxx"
#include "pugl/pugl.h"
#include <cairo/cairo.h>

// general C++ includes
#include <list>
#include <vector>
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
    
    /// tells the UI a widget has captured a mouse-down event, and
    /// wants to be notified of mouse movement events
    void wantsMotionUpdates( Avtk::Widget* w, bool notifyOfMotion )
    {
      if( notifyOfMotion )
        motionUpdateWidget = w;
      else
        motionUpdateWidget = 0;
    }
    
    /// Initiate a drag-and-drop action, with the widget as the "origin widget",
    /// and the data is copied to the UI instance.
    void dragDropInit( Avtk::Widget* origin, size_t size, void* data );
    
    /// checks if the current drag-drop data-type is dropable on the "to" Widget
    bool dragDropVerify( Avtk::Widget* target );
    
    /// performs the drag-drop action
    void dragDropComplete( Avtk::Widget* target );
    
    
    
    /// Widget value callback: when a widget is added to the UI, its value
    /// callback is set to this function: it can be set to a custom function if
    /// preferred.
    ///
    /// If the Widget* returned from new Widget() is stored, it can be compared
    /// against @param widget, which allows executing code based which Widget
    /// caused the event.
    virtual void widgetValueCB( Avtk::Widget* widget) = 0;
    
    /// Static function for handling AVTK widget callbacks: is re-directed to
    /// instance-version above.
    static void staticWidgetValueCB( Avtk::Widget* widget, void* userdata)
    {
      UI* ui = (UI*)userdata;
      ui->widgetValueCB( widget );
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
    
    /// get the theme requested: the themes have ID's defined in theme file.
    /// calling this without a parameter returns the default theme.
    Theme* theme( int id = 0 );
    
    /// call this to recieve the LV2 widget handle
    PuglNativeWindow getNativeHandle()
    {
      // returns the X11 handle, or Win32 surface, or Quartz surface
      return puglGetNativeWindow( view );
    }
    
    /// when set, we're in a widgets callback, so don't do "callback" for other
    /// widgets: this avoids recurive value() function calling
    bool inValueCB;
  
  protected:
    PuglView* view;
    
    bool quit_;
    int w_, h_;
    
    /// the list of widgets currently instantiated, in order of being drawn.
    // Technically this is a list of yasper::ptr<Avtk::Widget> types, but they
    // act generally like raw pointers would do
    std::list< ptr<Avtk::Widget> > widgets;
    
    /// A list of themes, loaded on startup, which widgets can request
    std::vector< ptr<Avtk::Theme> > themes;
    
    /// pointers that are dynamically switched to represent a widget that could
    /// have a specific action performed with it in the future. These pointers
    /// *must* be checked for 0 before *any* use.
    Avtk::Widget* dragDropOrigin;
    Avtk::Widget* motionUpdateWidget;
    
    
    bool          dragDropTargetVerified;
    Avtk::Widget* dragDropTargetVerifiedWidget;
    
    size_t dragDropDataSize;
    char*  dragDropDataPtr;
    
    
    void scroll( int x, int y, int dx, int dy );
    void display( cairo_t* cr );
    void motion(int x, int y);
    void event( const PuglEvent* event );
    void close() { quit_ = true; }
    
    // Static Functions for handling PUGL events below
    static void onMotion(PuglView* view, int x, int y)
    {
      UI* ui = (UI*)puglGetHandle( view );
      ui->motion( x, y );
    }
    static void onClose(PuglView* view)
    {
      UI* ui = (UI*)puglGetHandle( view );
      ui->close();
    }
    static void onEvent(PuglView* view, const PuglEvent* event)
    {
      UI* ui = (UI*)puglGetHandle( view );
      ui->event( event );
    }
    static void onDisplay(PuglView* view)
    {
      UI* ui = (UI*)puglGetHandle( view );
      cairo_t* cr = (cairo_t*)puglGetContext(view);
      ui->display( cr );
    }
    static void onScroll(PuglView* view, int x, int y, float dx, float dy)
    {
      UI* ui = (UI*)puglGetHandle( view );
      ui->scroll( x, y, dx, dy );
    }
};

}; // namespace Avtk

#endif // OPENAV_AVTK_UI_HXX

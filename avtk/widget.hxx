
#ifndef OPENAV_AVTK_WIDGET_HXX
#define OPENAV_AVTK_WIDGET_HXX

#include <stdio.h>
#include <string>
#include <cairo/cairo.h>

#include "utils.hxx"
#include "helpers.hxx"
#include "pugl/pugl.h"


namespace Avtk
{

class UI;

class Widget
{
  public:
    Widget( Avtk::UI* ui_, int x_, int y_, int w_, int h_, std::string label_);
    virtual ~Widget(){}
    
    /// get the current value
    float value() { return value_; }
    
    /// set a new value, triggers "new-value" event
    void value( float v );
    
    virtual void draw( cairo_t* cr ) = 0;
    
    bool touches( int inx, int iny );
    
    /// called by the UI class on any event that occurs
    int handle( const PuglEvent* event );
    
    void drag( int x, int y );
    
    /// the callback and its userdata pointer
    void (*callback)(Widget* , void*);
    void* callbackUD;
    
    int x, y, w, h;         /// widget co-ords and size
    std::string label;      /// widget name - sometimes shown in UI
    float value_;           /// widget value
    
    /// 0 when no mouse button is down, otherwise the mouse button pressed
    int mouseButtonPressed_;
  
  protected:
    /// the Avtk::UI pointer, used to redraw the view etc
    Avtk::UI* ui;
    
    /// enum defines the way in which mouse click / drag works
    enum ClickDragMode {
      CDM_NONE,             /// clicking toggles value() as a bool
      CDM_DRAG_VERTICAL,    /// vertical mouse drag will change value()
      CDM_DRAG_HORIZONTAL,  /// horizontal mouse drag changes value()
    };
    
    void clickDragMode( ClickDragMode cdm );
    
    ClickDragMode cdm;
    int mX, mY;
    
};

};

#endif // OPENAV_AVTK_WIDGET_HXX

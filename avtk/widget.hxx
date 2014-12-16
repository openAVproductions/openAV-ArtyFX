
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
class Theme;
class Group;

class Widget
{
  public:
    Widget( Avtk::UI* ui_, int x_, int y_, int w_, int h_, std::string label_);
    virtual ~Widget(){}
    
    /// returns the name of the Widget
    const char* label(){return label_.c_str();}
    
    /// sets the visibility
    void visible( bool visibile );
    
    /// get the current value
    float value() { return value_; }
    
    /// set a new value, triggers "new-value" event
    void value( float v );
    
    virtual void draw( cairo_t* cr ) = 0;
    
    bool touches( int inx, int iny );
    
    /// called by the UI class on any event that occurs
    int handle( const PuglEvent* event );
    
    /// called by the UI class when this widget has a mouse pressed
    void motion( int x, int y );
    
    /// the callback and its userdata pointer
    void (*callback)(Widget* , void*);
    void* callbackUD;
    
    int x, y, w, h;         /// widget co-ords and size
    std::string label_;      /// widget name - sometimes shown in UI
    bool  visible_;         /// widget visibility
    float value_;           /// widget value
    
    /// 0 when no mouse button is down, otherwise the mouse button pressed
    int mouseButtonPressed_;
    
    /// the Avtk::UI pointer, used to redraw the view etc
    Avtk::UI* ui;
    
    /// sets a theme for a Widget
    void theme( Theme* t );
    
    void parent( Group* parent );
  
  protected:
    Avtk::Group* parent_;
    
    /// local Theme pointer: themes are loaded at startup, and maintained until
    /// quitting, allowing for optimized redraws.
    Avtk::Theme* theme_;
    
    enum ClickMode {
      CLICK_NONE,           /// click has no effect
      CLICK_TOGGLE,         /// click toggles value between zero and one
      CLICK_VALUE_FROM_Y,   /// click sets value to Y
    };
    
    /// sets the click mode: add a size (in px) to translate px -> value()
    void clickMode( ClickMode cm, int clickModeSize = 0 );
    ClickMode cm;
    int clickModeSize;
    
    /// enum defines the way in which mouse click / drag works
    enum DragMode {
      DM_NONE,              /// drag has no effect
      DM_DRAG_VERTICAL,     /// vertical mouse drag will change value()
      DM_DRAG_HORIZONTAL,   /// horizontal mouse drag changes value()
    };
    
    void dragMode( DragMode cdm );
    DragMode dm;
    
    /// used for mouse-drag
    int mX, mY;
    
    /// control scroll operations
    bool scrollDisable;
    bool scrollInvert;
};

};

#endif // OPENAV_AVTK_WIDGET_HXX

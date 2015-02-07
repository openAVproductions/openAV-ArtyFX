
#ifndef OPENAV_AVTK_WIDGET_HXX
#define OPENAV_AVTK_WIDGET_HXX

#include "common.hxx"

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
    virtual ~Widget();
    
    /// returns the name of the Widget
    const char* label(){return label_.c_str();}
    void label( const char* l ){label_ = l;}
    
    /// returns the widgets parent
    Group* parent(){return parent_;}
    
    /// sets the visibility: virtual so group can override to also mark children
    virtual void visible( bool visibile );
    virtual bool visible(){return visible_;}
    
    /// get the current value
    float value();
    /// set a new value and redraws widget
    void value( float v );
    
    /// sets a default value: right clicking will return the dial to this value
    // TODO
    /// unless the right-click mode is changed
    void defaultValue( float dv );
    
    /// can be used to change the way the widget behaves for its value().
    /// - FLOAT_0_1, value returns float value 0-1 (default)
    /// - VALUE_INT returns *any* integer value. example: Number, Number::setRange()
    /// parameters set the range of the widget
    enum ValueMode {
      VALUE_FLOAT_0_1 = 0,
      VALUE_INT,
    };
    void valueMode( ValueMode v, int base, int range );
    
    
    virtual void draw( cairo_t* cr ) = 0;
    
    bool touches( int inx, int iny );
    
    /// called by the UI class on any event that occurs
    virtual int handle( const PuglEvent* event );
    
    /// called by the UI class when this widget has a mouse pressed
    void motion( int x, int y );
    
    /// position of widget functions, and virtual set methods
    virtual int x(){return x_;}
    virtual int y(){return y_;}
    virtual int w(){return w_;}
    virtual int h(){return h_;}
    
    virtual void x(int x__){ x_ = x__; }
    virtual void y(int y__){ y_ = y__; }
    virtual void w(int w__){ w_ = w__; }
    virtual void h(int h__){ h_ = h__; }
    
    std::string label_;     /// widget name - sometimes shown in UI
    bool  visible_;         /// widget visibility
    
    enum ClickMode {
      CLICK_NONE,           /// click has no effect
      CLICK_MOMENTARY,      /// click highlights button while mouse-down
      CLICK_TOGGLE,         /// click toggles value between zero and one
      CLICK_VALUE_FROM_Y,   /// click sets value to mouse Y position / widget Y
    };
    
    enum RClickMode {
      RCLICK_NONE,          /// Click has no effect
      RCLICK_VALUE_DEFAULT, /// Resets the value() to the widgets default
    };
    
    /// sets the click mode
    void clickMode ( ClickMode  cm  );
    void rClickMode( RClickMode rcm );
    
    /// 0 when no mouse button is down, otherwise the mouse button pressed
    int mouseButton(){return mouseButtonPressed_;}
    int mouseX(){return mousePressX;}
    int mouseY(){return mousePressY;}
    
    /// the Avtk::UI pointer, used to redraw the view etc
    Avtk::UI* ui;
    
    /// sets a theme for a Widget
    void theme( Theme* t );
    
    void addToGroup( Group* parent, int itemNumber );
    int groupItemNumber(){return groupItemNumber_;}
    
    /// the callback and its userdata pointer. the user-data pointer is set to
    /// the Avtk::UI* that is passed into the contstructor. The callback 
    void (*callback)(Widget* , void*);
    void* callbackUD;
  
  protected:
    /// constructor for top level windows
    Widget( Avtk::UI* ui, int w, int h );
    
    Avtk::Group* parent_;
    
    /// widget co-ords and size
    int x_, y_, w_, h_;
    
    /// the original position of the widget. Used to calculate position and size
    /// of widget when rescaling UI
    const int initX, initY, initW, initH;
    
    /// local Theme pointer: themes are loaded at startup, and maintained until
    /// quitting, allowing for optimized redraws.
    Avtk::Theme* theme_;
    
    /// widget doesn't take any input: UI passes it along to other widgets
    bool noHandle_;
    
    /// group item number: useful for radio buttons and lists of selectable items
    bool groupChild;
    int groupItemNumber_;
    
    /// enum defines the way in which mouse click / drag works
    enum DragMode {
      DM_NONE,              /// drag has no effect
      DM_DRAG_VERTICAL,     /// vertical mouse drag will change value()
      DM_DRAG_HORIZONTAL,   /// horizontal mouse drag changes value()
    };
    
    void dragMode( DragMode cdm );
    DragMode dragMode(){return dm;}
    ClickMode clickMode(){return cm;}
    RClickMode rclickMode(){return rcm;}
    
    /// used for mouse-drag
    int mX, mY;
    
    /// holds the currently pressed mouse-button, protected so group widgets can
    /// update their own button press etc
    int mouseButtonPressed_;
    int mousePressX;
    int mousePressY;
    
    /// control scroll operations
    bool scrollDisable;
    bool scrollInvert;
    
    /// sets the scroll-delta amount: that is the size of the change that occurs
    /// to value() in the widget. PX moved / <value> == step
    void setScrollDeltaAmount( float sda );
    
  
  private:
    ClickMode cm;
    RClickMode rcm;
    DragMode  dm;
    ValueMode vm;
    
    int valueIntBase;
    int valueIntRange;
    
    /// widgets current value, to get/set use value() and value( float )
    float value_;
    float defaultValue_;
    float auditionValue_;
    
    float scrollDeltaAmount;

#ifdef AVTK_DEBUG
  public:
    // public so main() can print out how many widgets have not been cleaned up
    // (if any) at the end of a test run
    static int widgetCounter;
#endif
};

};

#endif // OPENAV_AVTK_WIDGET_HXX


#ifndef OPENAV_AVTK_WIDGET_HXX
#define OPENAV_AVTK_WIDGET_HXX

#include <stdio.h>
#include <string>
#include <cairo/cairo.h>

#include "utils.hxx"
#include "theme.hxx"
#include "helpers.hxx"

namespace Avtk
{

class Widget
{
  public:
    Widget(int x_, int y_, int w_, int h_, std::string label_) :
      x( x_ ),
      y( y_ ),
      w( w_ ),
      h( h_ ),
      label( label_ ),
      value_( 0 ),
      mouseButtonPressed_(0),
      
      callback( 0 ),
      callbackUD( 0 )
    {
      if( theme == 0 )
      {
        theme = new Theme();
      }
    }
    
    /// get the current value
    float value()
    {
      return value_;
    }
    
    /// set a new value, triggers "new-value" event
    void value( float v )
    {
      value_ = v;
    }
    
    virtual void draw( cairo_t* cr ) = 0;
    
    virtual ~Widget(){}
    
    bool touches( int inx, int iny, bool doCallback = true )
    {
      bool t = ( inx >= x && inx <= x + w && iny >= y && iny <= y + h);
      
      // call the callback if its set, and not told not to
      if ( doCallback && callback )
        callback( this, callbackUD );
      
      return t;
    }
    
    int x, y, w, h;         /// widget co-ords and size
    std::string label;      /// widget name - sometimes shown in UI
    float value_;           /// widget value
    
    // 0 when no mouse button is down, otherwise the mouse button pressed
    int mouseButtonPressed_;
    
    static Theme* theme;
    
    void (*callback)(Widget* , void*);
    void* callbackUD;
};

};

#endif // OPENAV_AVTK_WIDGET_HXX

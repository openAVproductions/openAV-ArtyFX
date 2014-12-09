
#include "widget.hxx"

#include "ui.hxx"
#include "theme.hxx"

namespace Avtk
{

Widget::Widget( Avtk::UI* ui_, int x_, int y_, int w_, int h_, std::string label_) :
  ui(ui_),
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
}

int Widget::handle( const PuglEvent* event )
{
  switch (event->type)
  {
    case PUGL_BUTTON_PRESS:
      {
        if ( event->button.x == 0 && event->button.y == 0 )
          return 0;
        
        if( touches( event->button.x, event->button.y ) )
        {
          value( !value() );
          printf("touches widget, new value %f\n", value() );
          ui->redraw();
        }
      } break;
    default:
      return 0; break;
  }
  return 0;
}

void Widget::value( float v )
{
  value_ = v;
  
  // call the callback if its set, and not told not to
  if ( true && callback )
    callback( this, callbackUD );
}

bool Widget::touches( int inx, int iny )
{
  return ( inx >= x && inx <= x + w && iny >= y && iny <= y + h);
}

}; // Avtk

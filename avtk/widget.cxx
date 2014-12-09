
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
  callbackUD( 0 ),
  
  cdm( CDM_NONE ),
  mX(0),
  mY(0)
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
          if( cdm == CDM_NONE )
            value( !value() );
          else if ( cdm == CDM_DRAG_VERTICAL ||
                    cdm == CDM_DRAG_HORIZONTAL )
          {
            // sample the vertical mouse position, drag events affect += value()
            mX = event->button.x;
            mY = event->button.y;
            
            // tell the UI that the current widget wants motion notify
            ui->wantsMotionUpdates( this, true );
          }
          
          printf("touches widget, new value %f\n", value() );
          ui->redraw();
        }
      } break;
    case PUGL_BUTTON_RELEASE:
      {
        ui->wantsMotionUpdates( this, false );
      }
    default:
      return 0; break;
  }
  return 0;
}

void Widget::drag( int x, int y )
{
  float delta = ( mY - y ) / float(h);
  
  if ( cdm == CDM_DRAG_HORIZONTAL )
    delta = ( x - mX ) / float(w);
  
  value( value_ + delta );
  //printf("drag(), delta %i, new value %\n", delta, value() );
  
  mX = x;
  mY = y;
  
  ui->redraw();
}

void Widget::value( float v )
{
  if( v > 1.0 ) v = 1.0;
  if( v < 0.0 ) v = 0.0;
  
  value_ = v;
  
  // call the callback if its set, and not told not to
  if ( true && callback )
    callback( this, callbackUD );
}

bool Widget::touches( int inx, int iny )
{
  return ( inx >= x && inx <= x + w && iny >= y && iny <= y + h);
}

void Widget::clickDragMode( ClickDragMode c )
{
  cdm = c;
}

}; // Avtk


#include "widget.hxx"

#include "ui.hxx"
#include "theme.hxx"

namespace Avtk
{
#ifdef AVTK_DEBUG
int Widget::widgetCounter = 0;
#endif

// constructor for top-level windows only
Widget::Widget( Avtk::UI* ui_, int w, int h ) :
  ui( ui_ ),
  noHandle_( false ),
  label_("avtk-top-level"),
  x_( 0 ),
  y_( 0 ),
  w_( w ),
  h_( h ),
  initX( 0 ),
  initY( 0 ),
  initW( w ),
  initH( h ),
  visible_(true),
  parent_( 0x0 ) // top levels don't have a parent
{
#ifdef AVTK_DEBUG
  widgetCounter++;
#endif
}

Widget::Widget( Avtk::UI* ui_, int x, int y, int w, int h, std::string label__) :
  ui(ui_),
  parent_( 0 ),
  theme_( ui->theme() ),
  
  noHandle_( false ),
  groupChild( false ),
  groupItemNumber_( -1 ),
  
  x_( x ),
  y_( y ),
  w_( w ),
  h_( h ),
  initX( x ),
  initY( y ),
  initW( w ),
  initH( h ),
  
  label_( label__ ),
  visible_( true ),
  
  value_( 0 ),
  
  callback( Avtk::UI::staticWidgetValueCB ),
  callbackUD( ui_ ),
  
  mouseButtonPressed_(0),
  
  cm( CLICK_NONE ),
  dm( DM_NONE ),
  vm( VALUE_FLOAT_0_1 ),
  
  mX(0),
  mY(0),
  scrollDisable( 1 ),
  scrollInvert( 0 ),
  // actual scroll in PX / number == delta
  scrollDeltaAmount( 10 )
{
#ifdef AVTK_DEBUG
  widgetCounter++;
#endif
  
  
  /// add the widget to the top of the parentStack
  /// parentStack is a stack of Group* widgets. group->end() pops from stack
  //printf("Widget() %s : adding this to %s\n", label(), ui->parentStackTop()->label() );
  ui->parentStackTop()->add( this );
}

void Widget::theme( Theme* t )
{
  theme_ = t;
}

int Widget::handle( const PuglEvent* event )
{
  // eg: noHandle means this widget doesn't take any input.
  //     !visible_ implies the widget isn't shown: so a user can't interact with it
  if( noHandle_ || !visible_ )
  {
#ifdef AVTK_DEBUG
    printf("widget %s noHandle (%i) or visible (%i)\n", label(), int(noHandle_), int(visible_) );
#endif
    // no point in calling motion() on a widget that isn't shown, or doesn't handle
    ui->wantsMotionUpdates( this, false );
    return 0;
  }
  switch (event->type)
  {
    case PUGL_BUTTON_PRESS:
      {
        if ( event->button.x == 0 && event->button.y == 0 )
          return 0;
        
        if( touches( event->button.x, event->button.y ) )
        {
          mouseButtonPressed_ = event->button.button;
          mousePressX = event->button.x;
          mousePressY = event->button.y;
#ifdef AVTK_DEBUG
          printf("click touches %s, clickMode %i, mouseBtn %i\n", label_.c_str(), clickMode(), mouseButton() );
#endif // AVTK_DEBUG
          if( cm == CLICK_TOGGLE )
          {
            value( !value() );
            callback( this, callbackUD );
            ui->redraw( this );
          }
          else if ( cm == CLICK_MOMENTARY )
          {
            value( 1 );
            callback( this, callbackUD );
            ui->redraw( this );
          }
          else if ( cm == CLICK_VALUE_FROM_Y )
          {
            float tmp = (event->button.y - y_) / h_/0.92;
            value( tmp );
#ifdef AVTK_DEBUG
            printf("Widget::handle() value from Y, %f\n", tmp);
#endif
            callback( this, callbackUD );
            ui->redraw( this );
          }
          
          
          if( dm == DM_DRAG_VERTICAL ||
              dm == DM_DRAG_HORIZONTAL )
          {
            // sample the vertical mouse position, drag events affect += value()
            mX = event->button.x;
            mY = event->button.y;
          }
          
          // tell the UI that the current widget wants motion notify updates
          // this also handles Drag-n-Drop, so we need motion updates even if we
          // don't have DM_DRAG_VERTICAL || DM_DRAG_HORIZONTAL
          ui->wantsMotionUpdates( this, true );
          return 1;
        }
      }
      break;
    
    case PUGL_BUTTON_RELEASE:
      {
        // FIXME: this is now run for each existing widget, until the touches()
        // widget is found, then event-propogation stops. Optimizte to avoid
        // repeated calling.
        ui->wantsMotionUpdates( this, false );
        
        //printf("click release %s, clickMode %i\n", label_.c_str(), clickMode() );
        if( touches( event->button.x, event->button.y ) )
        {
          //printf("Widget PUGL button release, cm %i\n", cm);
          
          if ( cm == CLICK_MOMENTARY )
          {
            value( 0 );
            ui->redraw();
#ifdef AVTK_DEBUG
            //printf("Widget MOMENTARY, redrawn value\n");
#endif
          }
          return 1;
        }
      }
      return 0;
      break;
    
    case PUGL_SCROLL:
      {
        bool scTch = touches( event->scroll.x, event->scroll.y );
        if( scTch && !scrollDisable )
        {
#ifdef AVTK_DEBUG
          printf("scroll touch %i, x %lf, y %lf\n", int(scTch), event->scroll.x, event->scroll.y );
#endif
          float delta = event->scroll.dy / float(scrollDeltaAmount);
          if( scrollInvert )
            delta = -delta;
          value( value() + delta );
          callback( this, callbackUD );
          ui->redraw( this );
          return 1;
        }
      } break;
    
    case PUGL_KEY_PRESS:
      {
        PuglEventKey* e = (PuglEventKey*)event;
        if( touches( e->x, e->y ) )
        {
          if (event->key.character == ' ')
          {
            //printf("pugl space\n");
            callback( this, callbackUD );
          }
          else if (event->key.character == 's')
          {
            /*
            printf("pugl key s\n");
            float delta = 1 / 10.f;
            value( value_ - delta );
            ui->redraw( this );
            return 1;
            */
          }
          break;
        }
      }
    
    default:
      return 0; break;
  }
  
  return 0;
}

void Widget::motion( int inX, int inY )
{
  if ( dm == DM_NONE )
  {
    // if widget is pressed, and mouse moves outside the widget area
    // inform UI of possible drag-drop action
    if( !touches( inX, inY ) )
    {
      static const char* testData = "DragDropTestPayload";
#ifdef AVTK_DEBUG
      printf("motion outside widget -> DND?\n");
#endif // AVTK_DEBUG
      ui->dragDropInit( this, strlen( testData ), (void*)testData );
    }
    return;
  }
  
  // handle value() on the widget
  float delta = 0;
  float dragSpeed = float(h_);
  if ( dm == DM_DRAG_VERTICAL )
  {
    if( dragSpeed < 100 )
    {
      dragSpeed = 100; // num of px for "full-scale" drag
      //printf("dragspeed set to %f\n", dragSpeed);
    }
    
    delta = ( mY - inY ) / dragSpeed;
  }
  else if ( dm == DM_DRAG_HORIZONTAL )
  {
    dragSpeed = float(w_);
    if( dragSpeed < 100 )
    {
      dragSpeed = 100; // num of px for "full-scale" drag
      //printf("dragspeed set to %f\n", dragSpeed);
    }
    delta = ( inX - mX ) / dragSpeed;
  }
  
  value( value_ + delta );
  //printf("drag(), delta %f, new value %f\n", delta, value() );
  
  mX = inX;
  mY = inY;
  
  // check types of "when()" here?
  // immidiate
  // on-release
  if( true )
    callback( this, callbackUD );
  
  ui->redraw( this );
}

void Widget::valueMode( ValueMode v, int base, int range )
{
  vm = v;
  valueIntBase = base;
  valueIntRange = range;
  setScrollDeltaAmount( 1 );
}

void Widget::setScrollDeltaAmount( float sda )
{
  scrollDeltaAmount = sda;
}

float Widget::value()
{
  if( vm == VALUE_FLOAT_0_1 )
    return value_;
  
  return (value_ * valueIntRange) + valueIntBase;
}

void Widget::value( float v )
{
  if( vm == VALUE_INT )
  {
    //value_ * valueIntRange + valueIntBase;
    float tmp = (v-valueIntBase) / float(valueIntRange);
    
    printf("VALUE_INT input %f, internal value %f\n", v, tmp );
    
    v = tmp;
  }
  
  if( v > 1.0 ) v = 1.0;
  if( v < 0.0 ) v = 0.0;
  
  value_ = v;
  
#ifdef AVTK_DEBUG
  printf("Widget %s  value() %f\n", label_.c_str(), v );
#endif
  ui->redraw();
}

bool Widget::touches( int inx, int iny )
{
  return ( inx >= x_ && inx <= x_ + w_ && iny >= y_ && iny <= y_ + h_);
}

void Widget::clickMode( ClickMode c )
{
  cm = c;
#ifdef AVTK_DEBUG
  //printf("Widget %s  clickMode %i, %i\n", label_.c_str(), cm, c);
#endif // AVTK_DEBUG
}

void Widget::visible( bool v )
{
  visible_ = v;
}

void Widget::addToGroup( Group* p, int gin )
{
#ifdef AVTK_DEBUG
  printf("%s adding %s to %s\n", __PRETTY_FUNCTION__, label(), p->label() );
#endif
  groupChild = true;
  parent_ = p;
  groupItemNumber_ = gin;
}

void Widget::dragMode( DragMode d )
{
  dm = d;
}

Widget::~Widget()
{
#ifdef AVTK_DEBUG
  widgetCounter--;
  printf("widgetCounter = %i\n", widgetCounter );
#endif
  
#ifdef AVTK_DEBUG_DTOR
  printf("%s %s\n", __PRETTY_FUNCTION__, label() );
#endif // AVTK_DEBUG
}

}; // Avtk


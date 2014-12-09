
#include "widget.hxx"

#include "ui.hxx"
#include "theme.hxx"

Avtk::Theme* Avtk::Widget::theme = 0;

int Avtk::Widget::handle( const PuglEvent* event )
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


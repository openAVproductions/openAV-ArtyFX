
#include "dial_test.hxx"

#include "../avtk/theme.hxx"

#include <sstream>

#define WIDTH  120
#define HEIGHT  60

DialUI::DialUI(PuglNativeWindow parent) :
  Avtk::UI( WIDTH, HEIGHT, parent, "AVTK : Event Editor" )
{
  themes.push_back( new Avtk::Theme( this, "default.avtk" ) );
  Widget::theme_ = themes.front();
  
  Avtk::Widget* w = 0;
  
  dial1 = new Avtk::Dial( this, 10, 10, WIDTH/2-20, HEIGHT-20, "Dial" );
  
  dial2 = new Avtk::Dial( this, WIDTH/2+10, 10, WIDTH/2-20, HEIGHT-20, "Dial" );
  dial2->rClickMode( Widget::RCLICK_NONE );
  
  dial2 = new Avtk::Dial( this, WIDTH/3+00, 10, WIDTH/2-20, HEIGHT-20, "Dial" );
  dial2->rClickMode( Widget::RCLICK_NONE );
}

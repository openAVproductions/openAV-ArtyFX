
#include "dial_test.hxx"

#include "../avtk/theme.hxx"

#include <sstream>

#define WIDTH  60
#define HEIGHT 60

DialUI::DialUI(PuglNativeWindow parent) :
  Avtk::UI( WIDTH, HEIGHT, parent, "AVTK : Event Editor" )
{
  themes.push_back( new Avtk::Theme( this, "default.avtk" ) );
  Widget::theme_ = themes.front();
  
  Avtk::Widget* w = 0;
  
  dial = new Avtk::Dial( this, 10, 10, WIDTH-20, HEIGHT-20, "Dial" );
}

 

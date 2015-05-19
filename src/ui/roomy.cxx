
#include "roomy.hxx"

#include "../avtk/theme.hxx"

#include <sstream>

#define WIDTH  160
#define HEIGHT 220

RoomyUI::RoomyUI(PuglNativeWindow parent) :
  Avtk::UI( WIDTH, HEIGHT, parent, "Roomy : ArtyFX : OpenAV" )
{
  Avtk::Widget* w = 0;
  w = new Avtk::Button( this, 0, 0, 160,  29, "header");
  w = new Avtk::Button( this, 5,36, 150, 126, "graph" );
  
  dial1 = new Avtk::Dial( this, 10, 172, 40,40, "Time" );
  dial2 = new Avtk::Dial( this, 50, 172, 40,40, "Damp" );
  dial3 = new Avtk::Dial( this, 90, 172, 40,40, "Dry Wet" );
}

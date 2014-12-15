
#include "test_ui.hxx"
#include "avtk/utils.hxx"
#include "avtk/theme.hxx"

#include <sstream>

static void roundCB(Avtk::Widget* w, void* ud);
static void dialCB(Avtk::Widget* w, void* ud);
static void widgetCB(Avtk::Widget* w, void* ud);
static void zoomCB(Avtk::Widget* w, void* ud);
static void zoomOffsetCB(Avtk::Widget* w, void* ud);
static void listCB(Avtk::Widget* w, void* ud);

TestUI::TestUI( PuglNativeWindow parent ):
  Avtk::UI( 610, 430, parent )
{
  // slider vert
  Avtk::Widget* w = new Avtk::Slider( this, 520, 40, 22, 220, "Vertical Slider" );
  w->callback = widgetCB;
  w->callbackUD = this;
  add( w );
  
  // button
  w = new Avtk::Button( this, 3, 45, 90, 22, "Button" );
  //w->callback = widgetCB;
  w->callbackUD = this;
  add( w );
  
  // dial
  w = new Avtk::Dial( this, 75, 75, 75, 75, "Dial 1" );
  //w->callback = widgetCB;
  w->callbackUD = this;
  add( w );
  
  // list
  list = new Avtk::List( this, 345, 45, 75, 125, "List (Left)" );
  list->callback   = listCB;
  list->callbackUD = this;
  add( list );
  
  list2 = new Avtk::List( this, 425, 45, 75, 125, "List (Right)" );
  add( list2 );
  
  // waveform
  waveform = new Avtk::Waveform( this, 75, 175, 375, 125, "Waveform" );
  //waveform->callback = widgetCB;
  //waveform->callbackUD = this;
  
  std::vector<float> tmp;
  int error = Avtk::loadSample( "test.wav", tmp );
  waveform->show( tmp );
  add( waveform );
  
  w = new Avtk::Envelope( this, 215, 115, 60, 40, "Envelope" );
  //w->callback = widgetCB;
  w->callbackUD = this;
  add( w );
  
  // image
  Avtk::Image* i = new Avtk::Image( this, 0, 0, 610, 36, "Image" );
  i->load( header.pixel_data );
  add( i );
  
  // slider horizontal
  w =  new Avtk::Slider( this,  40,350, 350, 22, "Zoom" );
  w->callback   = zoomCB;
  w->callbackUD = this;
  add( w );
  
  w =  new Avtk::Slider( this,  40,374, 350, 22, "Vol" );
  w->callback   = zoomOffsetCB;
  w->callbackUD = this;
  add( w );
  
}

static void zoomOffsetCB(Avtk::Widget* w, void* ud)
{
  TestUI* ui = (TestUI*)ud;
  ((Avtk::Waveform*)ui->waveform)->zoomOffset( w->value() );
}

static void zoomCB(Avtk::Widget* w, void* ud)
{
  TestUI* ui = (TestUI*)ud;
  ((Avtk::Waveform*)ui->waveform)->zoom( 1 + w->value() * 10 );
}

static void dialCB(Avtk::Widget* w, void* ud)
{
  printf( "dialCB(), rad = %i\n", w->value() );
}

static void listCB(Avtk::Widget* w, void* ud)
{
  TestUI* ui = (TestUI*)ud;
  
  /*
  int t = w->value();
  
  
  for(int i = 0; i < w->value(); i++ )
  {
    std::stringstream s;
    s << w->value();
    tmp.push_back( s.str() );
  }
  */
  
  std::vector< std::string > tmp;
  
  int error = Avtk::directoryContents("/root/openav/avtk", tmp );
  
  ((Avtk::List*)ui->list2)->show( tmp );
  
}

static void widgetCB(Avtk::Widget* w, void* ud)
{
  /*
  if( w->theme->cornerRadius() < 4 )
    w->theme->cornerRadius( 4 );
  else
    w->theme->cornerRadius( 2 );
  */
  //printf( "widgetCB(), rad = %i\n", w->theme->cornerRadius() );
  
  TestUI* ui = (TestUI*)ud;
  
  ui->setAllWidgets( w, w->value() );
  
  /*
  std::vector< std::string > files;
  
  Avtk::directoryContents( "/root", files );
  
  for( int i = 0; i < files.size(); i++)
    printf("%i : %s\n", i, files.at(i).c_str() );
  */
}

void TestUI::setAllWidgets( Avtk::Widget* w, float v )
{
  originSetAllWidget = w;
  printf( "setAllWidgets(), value = %f\n", v );
  
  for (std::list< ptr<Avtk::Widget> >::iterator it = widgets.begin()++; it != widgets.end(); it++ )
  {
    // exclude sliders / drag widgets, since it'll act weird
    if( (*it) != originSetAllWidget )
    {
      (*it)->value( v );
    }
  }
  redraw();
}


#include "test_ui.hxx"
#include "avtk/utils.hxx"
#include "avtk/theme.hxx"

#include "libs/audioGen.hxx"

#include <sstream>

static void roundCB(Avtk::Widget* w, void* ud);
static void dialCB(Avtk::Widget* w, void* ud);
static void widgetCB(Avtk::Widget* w, void* ud);
static void zoomCB(Avtk::Widget* w, void* ud);
static void zoomOffsetCB(Avtk::Widget* w, void* ud);
static void listCB(Avtk::Widget* w, void* ud);
static void listValueCB(Avtk::Widget* w, void* ud);
static void toggleCB(Avtk::Widget* w, void* ud);

TestUI::TestUI( PuglNativeWindow parent ):
  Avtk::UI( 810, 530, parent )
{
  themes.push_back( new Avtk::Theme( this, "orange.avtk" ) );
  themes.push_back( new Avtk::Theme( this, "green.avtk" ) );
  themes.push_back( new Avtk::Theme( this, "yellow.avtk" ) );
  
  Widget::theme_ = themes.front();
  
  Avtk::Widget* w = 0;
  
  // group testing
  group1 = new Avtk::Group( this, 660, 43, 100, 0, "Group 1" );
  group1->mode( Avtk::Group::WIDTH_EQUAL );
  group1->valueMode ( Group::VALUE_SINGLE_CHILD );
  group1->resizeMode( Group::RESIZE_FIT_TO_CHILDREN );
  
  w = new Avtk::ListItem( this, 7, 45, 90, 11, "Group Toggle 1" );
  group1->add( w );
  w = new Avtk::ListItem( this, 7, 45, 90, 11, "Group Toggle 2" );
  group1->add( w );
  w = new Avtk::ListItem( this, 7, 45, 90, 11, "Group Toggle 3" );
  group1->add( w );
  w = new Avtk::ListItem( this, 7, 45, 90, 11, "Group Toggle 4" );
  group1->add( w );
  w->value( 1 );
  w = new Avtk::ListItem( this, 7, 45, 90, 11, "Group Toggle 5" );
  group1->add( w );
  w = new Avtk::ListItem( this, 7, 45, 90, 11, "Group Toggle 11" );
  group1->add( w );
  w = new Avtk::ListItem( this, 7, 45, 90, 11, "Group Toggle 21" );
  group1->add( w );
  w = new Avtk::ListItem( this, 7, 45, 90, 11, "Group Toggle 31" );
  group1->add( w );
  w = new Avtk::ListItem( this, 7, 45, 90, 11, "Group Toggle 41" );
  group1->add( w );
  
  // Editor
  //scroll = new Avtk::Scroll( this, 130, 43, 120, 60, "Scroll 1" );
  /*
  int scale = 4;
  editor = new Avtk::EventEditor( this, 0, 0, 240*scale, 250*scale, "EventEditor" );
  editor->value( true );
  editor->visible( true );
  /*
  */
  //scroll->set( editor );
  //scroll->set( group1 );
  
  scroll = new Avtk::Scroll( this, 130, 43, 520, 210, "Scroll 1" );
  
  // list
  list = new Avtk::List( this, 345, 345, 105, 425, "List (Left)" );
  std::vector<std::string> items;
  std::string stripped;
  Avtk::directoryContents(  "/root/openav/content/", items, stripped);
  
  std::vector<std::string> items2;
  items2.push_back( items.at(0) );
  list->show( items );
  list->mode      ( Group::WIDTH_EQUAL );
  list->valueMode ( Group::VALUE_SINGLE_CHILD );
  //list->resizeMode( Group::RESIZE_FIT_TO_CHILDREN );
  
  scroll->set( list );
  //scroll->set( group1 );
  
  /*
  // slider vert
  vertSlider = new Avtk::Slider( this, 755,  40, 22, 320, "Vertical   Slider" );
  horiSlider = new Avtk::Slider( this, 130, 365, 620, 22, "Horizontal Slider" );
  
  vertSlider->value ( 1.0 );
  horiSlider->value ( 0.5 );
  scroll->vertical  ( 1.0 );
  scroll->horizontal( 0.5 );
  */
  
  
  // button
  momentary = new Avtk::Button( this, 7, 45, 90, 22, "Zoom In" );
  momentary->theme( theme( 1 ) );
  momentary->clickMode( Avtk::Widget::CLICK_MOMENTARY );
  
  
  momentaryOut = new Avtk::Button( this, 7, 69, 90, 22, "Zoom Out" );
  momentaryOut->theme( theme( 2 ) );
  //momentaryOut->clickMode( Avtk::Widget::CLICK_TOGGLE );
  momentaryOut->clickMode( Avtk::Widget::CLICK_MOMENTARY );
  
  
  /*
  
  // button
  groupToggler = new Avtk::Button( this, 7 + 100, 45, 130, 22, "Group Toggler" );
  groupToggler->theme( theme( 2 ) );
  groupToggler->clickMode( Avtk::Widget::CLICK_TOGGLE );
  
  // dial
  w = new Avtk::Dial( this, 7, 85, 75, 75, "Dial 1" );
  
  // number
  w = new Avtk::Number( this, 85, 85, 35, 25, "Number box" );
  
  items.clear();
  Avtk::directoryContents(  "/root/openav/content/bips", items, stripped, true, true );
  list2 = new Avtk::List( this, 525, 345, 105, 125, "List (Right)" );
  list2->show( items );
  
  
  // waveform
  waveform = new Avtk::Waveform( this, 15, 415, 250, 100, "Waveform" );
  std::vector<float> tmp;
  int error = Avtk::loadSample( "test.wav", tmp );
  waveform->show( tmp );
  */
  
  // spectrum
  spectrum = new Avtk::Spectrum( this, 150, 405, 250, 90, "Spectrum" );
  
  std::vector<float> audio;
  for(int i = 0; i < 44100; i++)
  {
    audio.push_back( 0 );
  }
  // gen audio
  sawtooth( 44100, &audio[0], 440, 50 );
  // analyse it
  audioDataSpectrum( 44100, &audio[0] );
  // set it to widget
  spectrum->show( 422, &audio[0] );
  
  /*
  w = new Avtk::Envelope( this, 215, 115, 60, 40, "Envelope" );
  
  // image
  Avtk::Image* i = new Avtk::Image( this, 0, 0, 610, 36, "Image" );
  i->load( header.pixel_data );
  
  /*
  // slider horizontal
  w =  new Avtk::Slider( this,  15,350, 250, 22, "Zoom" );
  w =  new Avtk::Slider( this,  15,374, 250, 22, "Vol" );
  */
}


void TestUI::widgetValueCB( Avtk::Widget* w )
{
  if( w == momentary )
  {
    editor->zoom( 1 );
    ((Avtk::Scroll*)editor->parent())->childResize( editor );
    //w = new Avtk::ListItem( this, 7, 45, 90, 11, "Group Toggle dyn" );
    //group1->add( w );
  }
  else if( w == momentaryOut )
  {
    editor->zoom( 0 );
    ((Avtk::Scroll*)editor->parent())->childResize( editor );
  }
  else if( w == vertSlider )
  {
    scroll->vertical( w->value() );
  }
  else if( w == horiSlider )
  {
    scroll->horizontal( w->value() );
  }
  else
  {
    printf( "%s, value = %f\n", w->label(), w->value() );
  }
}

static void listValueCB( Avtk::Widget* w, void* ud )
{
  Avtk::List* l = (Avtk::List*)w;

}

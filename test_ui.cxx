
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
  group1 = new Avtk::Group( this, 610, 43, 100, 120, "Group 1" );
  group1->mode( Avtk::Group::WIDTH_EQUAL );
  w = new Avtk::ListItem( this, 7, 45, 90, 11, "Group Toggle 1" );
  group1->add( w );
  w = new Avtk::ListItem( this, 7, 45, 90, 11, "Group Toggle 2" );
  group1->add( w );
  w = new Avtk::ListItem( this, 7, 45, 90, 11, "Group Toggle 3" );
  group1->add( w );
  w = new Avtk::ListItem( this, 7, 45, 90, 11, "Group Toggle 4" );
  group1->add( w );
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
  w = new Avtk::ListItem( this, 7, 45, 90, 11, "Group Toggle 51" );
  group1->add( w );
  
  group1->valueMode ( Group::VALUE_SINGLE_CHILD ); 
  group1->resizeMode( Group::RESIZE_FIT_TO_CHILDREN );
  
  
  // Editor
  int scale = 4;
  editor = new Avtk::EventEditor( this, 0, 0, 240*scale, 250*scale, "EventEditor" );
  editor->value( true );
  editor->visible( false );
  
  // scroller
  scroll = new Avtk::Scroll( this, 130, 43, 120, 60, "Scroll 1" );
  
  //scroll->set( editor );
  scroll->set( group1 );
  
  // slider vert
  vertSlider = new Avtk::Slider( this, 755,  40, 22, 320, "Vertical   Slider" );
  horiSlider = new Avtk::Slider( this, 130, 365, 620, 22, "Horizontal Slider" );
  
  vertSlider->value ( 0.5 );
  horiSlider->value ( 0.5 );
  scroll->vertical  ( 0.5 );
  scroll->horizontal( 0.5 );
  
  // button
  momentary = new Avtk::Button( this, 7, 45, 90, 22, "Zoom In" );
  momentary->theme( theme( 1 ) );
  momentary->clickMode( Avtk::Widget::CLICK_TOGGLE );
  
  momentaryOut = new Avtk::Button( this, 7, 69, 90, 22, "Zoom Out" );
  momentaryOut->theme( theme( 2 ) );
  momentaryOut->clickMode( Avtk::Widget::CLICK_TOGGLE );
  //momentary->clickMode( Avtk::Widget::CLICK_MOMENTARY );
  
  /*
  // button
  groupToggler = new Avtk::Button( this, 7 + 100, 45, 130, 22, "Group Toggler" );
  groupToggler->theme( theme( 2 ) );
  groupToggler->clickMode( Avtk::Widget::CLICK_TOGGLE );
  */
  
  // dial
  w = new Avtk::Dial( this, 7, 85, 75, 75, "Dial 1" );
  
  // number
  w = new Avtk::Number( this, 85, 85, 35, 25, "Number box" );
  
  /*
  // list
  list = new Avtk::List( this, 345, 345, 105, 125, "List (Left)" );
  std::vector<std::string> items;
  std::string stripped;
  Avtk::directoryContents(  "/root/openav/content/bips/", items, stripped);
  list->show( items );
  
  items.clear();
  Avtk::directoryContents(  "/root/openav/content/bips", items, stripped, true, true );
  list2 = new Avtk::List( this, 525, 345, 105, 125, "List (Right)" );
  list2->show( items );
  */
  
  // waveform
  waveform = new Avtk::Waveform( this, 15, 415, 250, 100, "Waveform" );
  std::vector<float> tmp;
  int error = Avtk::loadSample( "test.wav", tmp );
  waveform->show( tmp );
  
  //w = new Avtk::Envelope( this, 215, 115, 60, 40, "Envelope" );
  
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
  if( w == groupToggler )
  {
    //group1->visible( groupToggler->value() );
    //list2->visible( groupToggler->value() );
  }
  else if( w == momentary )
  {
    editor->zoom( 1 );
    ((Avtk::Scroll*)editor->parent())->childResize( editor );
    redraw();
  }
  else if( w == momentaryOut )
  {
    editor->zoom( 0 );
    ((Avtk::Scroll*)editor->parent())->childResize( editor );
    redraw();
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

/*
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
  //printf( "dialCB(), rad = %i\n", w->value() );
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
  *
  
  std::vector< std::string > tmp;
  
  int error = Avtk::directoryContents("/root/openav/avtk", tmp );
  
  ((Avtk::List*)ui->list2)->show( tmp );
  
}

static void widgetCB(Avtk::Widget* w, void* ud)
{
  
  //printf( "widgetCB(), rad = %i\n", w->theme->cornerRadius() );
  
  TestUI* ui = (TestUI*)ud;
  
  ui->setAllWidgets( w, w->value() );
  
  /*
  std::vector< std::string > files;
  
  Avtk::directoryContents( "/root", files );
  
  for( int i = 0; i < files.size(); i++)
    printf("%i : %s\n", i, files.at(i).c_str() );
  /
}

void TestUI::setAllWidgets( Avtk::Widget* w, float v )
{
  originSetAllWidget = w;
  
  for (std::list< ptr<Avtk::Widget> >::iterator it = widgets.begin()++; it != widgets.end(); it++ )
  {
    // exclude sliders / drag widgets, since it'll act weird
    if( (*it) != originSetAllWidget )
    {
      (*it)->value( v );
      (*it)->theme( theme( v * themes.size() ) );
    }
  }
  redraw();
}
*/

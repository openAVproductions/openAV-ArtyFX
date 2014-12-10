
#include "test_ui.hxx"
#include "avtk/theme.hxx"

static void roundCB(Avtk::Widget* w, void* ud);
static void widgetCB(Avtk::Widget* w, void* ud);


TestUI::TestUI():
  Avtk::UI( 610, 430 )
{
  // button
  Avtk::Widget* w = new Avtk::Button( this, 3, 45, 40, 22, "-" );
  w->callback = widgetCB;
  w->callbackUD = this;
  add( w );
  
  // image
  Avtk::Image* i = new Avtk::Image( this, 0, 0, 610, 36, "-" );
  i->load( header.pixel_data );
  add( i );
  
  // slider vert + horizontal
  w = new Avtk::Slider( this, 520, 40, 22, 220, "Rounded Corners" );
  w->callback = roundCB;
  w->callbackUD = this;
  add( w );
  add( new Avtk::Slider( this,  40,350, 350, 22, "Vol" ) );
}

static void roundCB(Avtk::Widget* w, void* ud)
{
  w->ui->theme->cornerRadius( w->value() * 11 );
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
  ui->redraw();
  
  /*
  std::vector< std::string > files;
  
  Avtk::directoryContents( "/root", files );
  
  for( int i = 0; i < files.size(); i++)
    printf("%i : %s\n", i, files.at(i).c_str() );
  */
}

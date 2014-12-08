
#ifndef OPENAV_AVTK_TEST_UI_HXX
#define OPENAV_AVTK_TEST_UI_HXX

#include "avtk/avtk.hxx"

#include "header.c"

// FIXME: refactor?
static void widgetCB(Avtk::Widget* w, void* ud);

class TestUI : public Avtk::UI
{
  public:
    TestUI():
      Avtk::UI( 610, 430 )
    {
      addRow( 0, 72 );
      
      Avtk::Image* w = new Avtk::Image( 0, 0, 610, 36, "-" );
      w->load( header.pixel_data );
      add( w );
      
      for(int i = 0; i < 16; i++){
        addRow( 75 + i * ( 3 + 22 ) );
      }
    }
    
    void addRow( int offset, int btnW = 22 )
    {
      for(int r = 0; r < 10; r++ )
      {
        Avtk::Widget* w = 0;
        int spcr =  3;
        int btnH = 22;
        w = new Avtk::Button( offset + spcr, 40 + ( btnH + spcr )* r, btnW, btnH, "-" );
        w->callback = widgetCB;
        w->callbackUD = this;
        add( w );
      }
      
    }
};

static void widgetCB(Avtk::Widget* w, void* ud)
{
  if( w->theme->cornerRadius() < 4 )
    w->theme->cornerRadius( 4 );
  else
    w->theme->cornerRadius( 2 );
  
  printf( "widgetCB(), rad = %i\n", w->theme->cornerRadius() );
  
  TestUI* ui = (TestUI*)ud;
  ui->redraw();
  
  /*
  std::vector< std::string > files;
  
  Avtk::directoryContents( "/root", files );
  
  for( int i = 0; i < files.size(); i++)
    printf("%i : %s\n", i, files.at(i).c_str() );
  */
}

#endif // OPENAV_AVTK_TEST_UI_HXX

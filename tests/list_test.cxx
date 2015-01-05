
#include "list_test.hxx"

#include "../avtk/theme.hxx"

#include <sstream>

#define WIDTH  500
#define HEIGHT 200

ListUI::ListUI(PuglNativeWindow parent) :
  Avtk::UI( WIDTH, HEIGHT, parent, "AVTK : List" )
{
  themes.push_back( new Avtk::Theme( this, "default.avtk" ) );
  Widget::theme_ = themes.front();
  
  
  listH = new Avtk::List( this, 10, 10, 100, 30, "List" );
  listH->mode( Group::HEIGHT_EQUAL );
  listH->resizeMode( Group::RESIZE_FIT_TO_CHILDREN );
  
  Avtk::Widget* w = 0;
  for(int i = 0; i < 5; i++ )
  {
    listH->addItem( "Tst" );
    //w = new Avtk::Button( this, 10, 10, WIDTH-20, HEIGHT-20, "List" );
    //listH->add( w );
  }
  
}


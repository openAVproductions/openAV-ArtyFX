
#include "dialog_test.hxx"

#include "../avtk/theme.hxx"

#include <stdio.h>
#include <sstream>

#define WIDTH  460
#define HEIGHT 260

DialogUI::DialogUI(PuglNativeWindow parent) :
  Avtk::UI( WIDTH, HEIGHT, parent, "AVTK : Event Editor" )
{
  themes.push_back( new Avtk::Theme( this, "default.avtk" ) );
  Widget::theme_ = themes.front();
  
  show = new Avtk::Button( this, 0, 0, WIDTH, HEIGHT, "Show Dialog");
  
  /// junk to check overlay
  Avtk::List* listH = new Avtk::List( this, 60, 10, 0, 30, "List" );
  listH->mode( Group::HEIGHT_EQUAL );
  listH->resizeMode( Group::RESIZE_FIT_TO_CHILDREN );
  
  Avtk::Widget* w = 0;
  for(int i = 0; i < 5; i++ )
  {
    listH->addItem( "Tst-H" );
  }
  
  
  dialog = new Avtk::Dialog( this, 60, 60, WIDTH-60*2, 100, "Dialog" );
}

void DialogUI::widgetValueCB( Avtk::Widget* widget)
{
  printf("widget dialog callback, mousebutton %i\n" );
  //dialog->visible( !dialog->visible() );
  //printf("visible %i\n", dialog->visible() );
  
  if( widget == show )
  {
    show->value( false );
    
    int mx = widget->mouseX();
    int my = widget->mouseY();
    
    int ret = dialog->run( "Header", "This is dialog text", Avtk::Dialog::OK_CANCEL, mx, my );
    printf("dialog returned %i\n", ret );
    
    
  }
}
 

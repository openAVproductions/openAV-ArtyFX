
#include <stdio.h>

#include "../avtk/widget.hxx"

#include "list_test.hxx"
#include "dial_test.hxx"
#include "group_test.hxx"
#include "resize_test.hxx"
#include "dialog_test.hxx"
#include "event_editor_test.hxx"

int main(int argc, char** argv)
{
  Avtk::UI* ui = 0;
  
  if( argc > 1 )
  {
    if( strcmp( argv[1], "dial") == 0 )
      ui = new DialUI();
    else if( strcmp( argv[1], "dialog") == 0 )
      ui = new DialogUI();
    else if( strcmp( argv[1], "group") == 0 )
      ui = new GroupUI();
    else if( strcmp( argv[1], "resize") == 0 )
      ui = new ResizeUI();
    else if( strcmp( argv[1], "list") == 0 )
      ui = new ListUI();
    else if( strcmp( argv[1], "eventeditor") == 0 )
      ui = new EventEditorUI();
    else
      printf("Invalid option %s\n", argv[1] );
  }
  
  if( ui )
  {
    ui->run();
    delete ui;
  }

#ifdef AVTK_DEBUG
  int widgetsOnExit = Avtk::Widget::widgetCounter;
  if( widgetsOnExit != 0 )
  {
    printf("%s : ERROR : widgetCounter == %i on exit!\n", argv[1], widgetsOnExit );
  }
#endif
  return 0;
}

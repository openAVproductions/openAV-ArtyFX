
#include <stdio.h>

#include "list_test.hxx"
#include "dial_test.hxx"
#include "resize_test.hxx"
#include "event_editor_test.hxx"

int main(int argc, char** argv)
{
  Avtk::UI* ui = 0;
  
  if( argc > 1 )
  {
    if( strcmp( argv[1], "dial") == 0 )
      ui = new DialUI();
    if( strcmp( argv[1], "resize") == 0 )
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
  return 0;
}

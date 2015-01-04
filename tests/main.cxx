
#include <stdio.h>

#include "dial_test.hxx"
#include "event_editor_test.hxx"

int main(int argc, char** argv)
{
  Avtk::UI* ui = 0;
  
  if( argc > 1 )
  {
    if( strcmp( argv[1], "dial") == 0 )
      ui = new DialUI();
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

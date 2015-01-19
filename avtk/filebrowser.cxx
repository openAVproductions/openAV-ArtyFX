
#include "filebrowser.hxx"

#include "ui.hxx"
#include "theme.hxx"

#include <stdio.h>

using namespace Avtk;

FileBrowser::FileBrowser( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Group( ui, x_, y_, w_, h_, label_ )
{
  
  
  
  end();
}

void FileBrowser::draw( cairo_t* cr )
{
  cairo_save( cr );
  
  
  
  cairo_restore( cr );
}


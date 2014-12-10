
#include "theme.hxx"

#include "ui.hxx"
#include "widget.hxx"

namespace Avtk
{

void Theme::cornerRadius( int c )
{
  cornerRadius_ = c;
  ui->redraw();
  printf("corner = %i\n", c );
}

float Theme::useDefaultColor( cairo_t* cr, USE_CASE uc, float alpha_ )
{
  switch( uc )
  {
    case BG:
      cairo_set_source_rgba(cr,  34/255.,  34/255.,  34/255., alpha_);
      break;
    case BG_DARK:
      cairo_set_source_rgba(cr,  17/255.,  17/255.,  17/255., alpha_);
      break;
  
    case FG:
      cairo_set_source_rgba(cr,  76/255.,  80/255.,  83/255., alpha_);
      break;
    case FG_DARK:
      cairo_set_source_rgba(cr,  35/255.,  87/255., 136/255., alpha_);
      break;
    
    case HIGHLIGHT:
      cairo_set_source_rgba(cr,   0/255., 128/255., 255/255., alpha_);
      break;
    
    case CORNER_RADIUS:
      return cornerRadius_;
      break;
    
    default:
      printf("Theme::useDefaultColor() color %i not handled!\n");
      break;
  };
}

}; // Avtk

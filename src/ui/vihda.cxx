
#include "vihda.hxx"
#include "../dsp/vihda.hxx"

#include "common.hxx"
#include "../avtk/theme.hxx"
#include "graphs/delay.hxx"
#include "headers/vihda.c"

VihdaUI::VihdaUI(PuglNativeWindow parent) :
  Avtk::UI( WIDTH, HEIGHT, parent, "Vihda (ArtyFX-OpenAV)" )
{
  Avtk::Image* i = new Avtk::Image( this, 0, 0, 160,  29, "header");
  i->load( vihda.pixel_data );
  
  rev   = new Avtk::Delay( this, 5,36, 150, 126, "graph" );
  
  dial1 = new Avtk::Dial( this,  8, 172, 45,45, "Width" );
  dial2 = new Avtk::Dial( this, 60, 172, 45,45, "Invert" );
}

void VihdaUI::widgetValueCB( Avtk::Widget* widget )
{
  float v = widget->value();
  //printf("Widget %s : %f\n", widget->label(), v );
  if( widget == dial1 )
  {
    rev->feedback = v;
    write_function( controller, VIHDA_WIDTH, sizeof(float), 0, &v );
  }
  if( widget == dial2 )
  {
    rev->volume = v;
    write_function( controller, VIHDA_INVERT, sizeof(float), 0, &v );
  }
  redraw();
}

void VihdaUI::lv2PortEvent( uint32_t index,
                            uint32_t buf_size,
                            uint32_t format,
                            const void* buffer )
{
  if( format != 0 )
    return;

  float v = *((float*)buffer);
  
  //printf("VihdaUI port() %i : v\n", index, v );
  
  switch( index )
  {
  case VIHDA_WIDTH:
    dial1->value( v );
    rev->feedback = v;
    break;
  case VIHDA_INVERT:
    dial2->value( v );
    rev->volume = v;
    break;
  }
  redraw();
  return;
}


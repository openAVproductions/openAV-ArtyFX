
#include "filta.hxx"
#include "../dsp/filta.hxx"

#include "common.hxx"
#include "../avtk/theme.hxx"
#include "graphs/filter.hxx"
#include "headers/filta.c"

FiltaUI::FiltaUI(PuglNativeWindow parent) :
  Avtk::UI( WIDTH, HEIGHT, parent, "Filta (ArtyFX-OpenAV)" )
{
  Avtk::Image* i = new Avtk::Image( this, 0, 0, 160,  29, "header");
  i->load( filta.pixel_data );
  
  graph = new Avtk::Filter( this, 5,36, 150, 126, "graph" );
  
  dial1 = new Avtk::Dial( this,  8, 172, 45,45, "Frequency" );
  //dial2 = new Avtk::Dial( this, 60, 172, 45,45, "Volume" );
  //dial3 = new Avtk::Dial( this,110, 172, 45,45, "Time" );
}

void FiltaUI::widgetValueCB( Avtk::Widget* widget )
{
  float v = widget->value();
  //printf("Widget %s : %f\n", widget->label(), v );
  if( widget == dial1 )
  {
    graph->value( v );
    write_function( controller, FILTA_FREQ_CONTROL, sizeof(float), 0, &v );
  }
  redraw();
}

void FiltaUI::lv2PortEvent( uint32_t index,
                            uint32_t buf_size,
                            uint32_t format,
                            const void* buffer )
{
  if( format != 0 )
    return;

  float v = *((float*)buffer);
  
  //printf("FiltaUI port() %i : v\n", index, v );
  
  switch( index )
  {
  case FILTA_FREQ_CONTROL:
    dial1->value( v );
    graph->value( v );
    break;
  }
  redraw();
  return;
}


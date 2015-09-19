
#include "satma.hxx"
#include "../dsp/satma.hxx"

#include "common.hxx"
#include "../avtk/theme.hxx"
#include "graphs/maximizer.hxx"
#include "headers/satma.c"

SatmaUI::SatmaUI(PuglNativeWindow parent) :
  Avtk::UI( WIDTH, HEIGHT, parent, "Satma (ArtyFX-OpenAV)" )
{
  Avtk::Image* i = new Avtk::Image( this, 0, 0, 160,  29, "header");
  i->load( satma.pixel_data );
  
  graph   = new Avtk::Maximizer( this, 5,36, 150, 126, "graph" );
  
  dial1 = new Avtk::Dial( this, 28, 169, 45,45, "Distortion" );
  dial2 = new Avtk::Dial( this, 96, 169, 45,45, "Tone" );
}

void SatmaUI::widgetValueCB( Avtk::Widget* widget )
{
  float v = widget->value();
  //printf("Widget %s : %f\n", widget->label(), v );
  if( widget == dial1 )
  {
    graph->value(v);
    write_function( controller, SATMA_DISTORTION, sizeof(float), 0, &v );
  }
  if( widget == dial2 )
  {
    graph->tone = v;
    write_function( controller, SATMA_TONE, sizeof(float), 0, &v );
  }
  redraw();
}

void SatmaUI::lv2PortEvent( uint32_t index,
                            uint32_t buf_size,
                            uint32_t format,
                            const void* buffer )
{
  if( format != 0 )
    return;

  float v = *((float*)buffer);
  
  //printf("SatmaUI port() %i : v\n", index, v );
  
  switch( index )
  {
  case SATMA_DISTORTION:
    dial1->value( v );
    graph->value( v );
    break;
  case SATMA_TONE:
    dial2->value( v );
    graph->tone = v;
    break;
  }
  redraw();
  return;
}


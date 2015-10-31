
#include "whaaa.hxx"
#include "../dsp/whaaa.hxx"

#include "common.hxx"
#include "../avtk/theme.hxx"
#include "graphs/wah.hxx"
#include "headers/whaaa.c"

WhaaaUI::WhaaaUI(PuglNativeWindow parent) :
  Avtk::UI( WIDTH, HEIGHT, parent, "Whaaa (ArtyFX-OpenAV)" )
{
  Avtk::Image* i = new Avtk::Image( this, 0, 0, 160,  29, "header");
  i->load( whaaa.pixel_data );
  
  graph = new Avtk::Wah( this, 5,36, 150, 126, "graph" );
  
  dial1 = new Avtk::Dial( this,  8, 172, 45,45, "Freq" );
  dial2 = new Avtk::Dial( this, 60, 172, 45,45, "Drive" );
  dial3 = new Avtk::Dial( this,110, 172, 45,45, "Mix" );
}

void WhaaaUI::widgetValueCB( Avtk::Widget* widget )
{
  float v = widget->value();
  //printf("Widget %s : %f\n", widget->label(), v );
  if( widget == dial1 )
  {
    graph->freq = v;
    write_function( controller, WHAAA_FREQ, sizeof(float), 0, &v );
  }
  if( widget == dial2 )
  {
    graph->drive = v;
    write_function( controller, WHAAA_DRIVE, sizeof(float), 0, &v );
  }
  if( widget == dial3 )
  {
    graph->mix = v;
    write_function( controller, WHAAA_MIX, sizeof(float), 0, &v );
  }
  redraw();
}

void WhaaaUI::lv2PortEvent( uint32_t index,
                            uint32_t buf_size,
                            uint32_t format,
                            const void* buffer )
{
  if( format != 0 )
    return;

  float v = *((float*)buffer);
  
  //printf("WhaaaUI port() %i : v\n", index, v );
  
  switch( index )
  {
  case WHAAA_FREQ:
    dial1->value( v );
    graph->freq = v;
    break;
  case WHAAA_DRIVE:
    dial2->value( v );
    graph->drive = v;
    break;
  case WHAAA_MIX:
    dial3->value( v );
    graph->mix = v;
    break;
  }
  redraw();
  return;
}


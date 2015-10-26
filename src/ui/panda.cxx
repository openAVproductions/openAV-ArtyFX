
#include "panda.hxx"
#include "../dsp/panda.hxx"

#include "common.hxx"
#include "../avtk/theme.hxx"
#include "graphs/compander.hxx"
#include "headers/panda.c"

PandaUI::PandaUI(PuglNativeWindow parent) :
  Avtk::UI( WIDTH, HEIGHT, parent, "Panda (ArtyFX-OpenAV)" )
{
  Avtk::Image* i = new Avtk::Image( this, 0, 0, 160,  29, "header");
  i->load( panda.pixel_data );
  
  graph = new Avtk::Compander( this, 5,36, 150, 126, "graph" );
  
  dial1 = new Avtk::Dial( this,  8, 172, 45,45, "Threshold" );
  dial3 = new Avtk::Dial( this, 60, 172, 45,45, "Release" );
  dial2 = new Avtk::Dial( this,112, 172, 45,45, "Factor" );
}

void PandaUI::widgetValueCB( Avtk::Widget* widget )
{
  float v = widget->value();
  //printf("Widget %s : %f\n", widget->label(), v );
  if( widget == dial1 )
  {
    graph->threshold = v;
    write_function( controller, PANDA_THRESHOLD, sizeof(float), 0, &v );
  }
  if( widget == dial2 )
  {
    graph->value(v);
    write_function( controller, PANDA_FACTOR, sizeof(float), 0, &v );
  }
  if( widget == dial3 )
  {
    graph->release = v;
    write_function( controller, PANDA_RELEASE, sizeof(float), 0, &v );
  }
  redraw();
}

void PandaUI::lv2PortEvent( uint32_t index,
                            uint32_t buf_size,
                            uint32_t format,
                            const void* buffer )
{
  if( format != 0 )
    return;

  float v = *((float*)buffer);
  
  //printf("PandaUI port() %i : v\n", index, v );
  
  switch( index )
  {
  case PANDA_THRESHOLD:
    dial1->value( v );
    graph->threshold = v;
    break;
  case PANDA_FACTOR:
    dial2->value( v );
    graph->value( v );
    break;
  case PANDA_RELEASE:
    dial3->value( v );
    graph->release = v;
    break;
  }
  redraw();
  return;
}



#include "ducka.hxx"
#include "../dsp/ducka.hxx"

#include "common.hxx"
#include "../avtk/theme.hxx"
#include "graphs/sidechain.hxx"
#include "headers/ducka.c"

DuckaUI::DuckaUI(PuglNativeWindow parent) :
  Avtk::UI( WIDTH, HEIGHT, parent, "Della (ArtyFX-OpenAV)" )
{
  Avtk::Image* i = new Avtk::Image( this, 0, 0, 160,  29, "header");
  i->load( ducka.pixel_data );
  
  graph   = new Avtk::Sidechain( this, 5,36, 150, 126, "graph" );
  
  dial1 = new Avtk::Dial( this,  8, 172, 45,45, "Thresh" );
  dial2 = new Avtk::Dial( this, 60, 172, 45,45, "Drop" );
  dial3 = new Avtk::Dial( this,110, 172, 45,45, "Time" );
}

void DuckaUI::widgetValueCB( Avtk::Widget* widget )
{
  float v = widget->value();
  //printf("Widget %s : %f\n", widget->label(), v );
  if( widget == dial1 )
  {
    graph->threshold = v;
    write_function( controller, DUCKA_THRESHOLD, sizeof(float), 0, &v );
  }
  if( widget == dial2 )
  {
    graph->reduceAmount = v;
    write_function( controller, DUCKA_REDUCTION, sizeof(float), 0, &v );
  }
  if( widget == dial3 )
  {
    graph->time = v;
    write_function( controller, DUCKA_RELEASE_TIME, sizeof(float), 0, &v );
  }
  redraw();
}

void DuckaUI::lv2PortEvent( uint32_t index,
                            uint32_t buf_size,
                            uint32_t format,
                            const void* buffer )
{
  if( format != 0 )
    return;

  float v = *((float*)buffer);
  
  //printf("DuckaUI port() %i : v\n", index, v );
  
  switch( index )
  {
  case DUCKA_THRESHOLD:
    dial1->value( v );
    graph->threshold = v;
    break;
  case DUCKA_REDUCTION:
    dial2->value( v );
    graph->reduceAmount = v;
    break;
  case DUCKA_RELEASE_TIME:
    dial3->value( v );
    graph->time = v;
    break;
  case DUCKA_SIDECHAIN_AMP:
    graph->value( v );
    break;
  }
  redraw();
  return;
}



#include "masha.hxx"
#include "../dsp/masha.hxx"

#include "common.hxx"
#include "../avtk/theme.hxx"
#include "graphs/masher.hxx"
#include "headers/masha.c"

MashaUI::MashaUI(PuglNativeWindow parent) :
  Avtk::UI( WIDTH, HEIGHT, parent, "Masha (ArtyFX-OpenAV)" )
{
  Avtk::Image* i = new Avtk::Image( this, 0, 0, 160,  29, "header");
  i->load( masha.pixel_data );
  
  graph = new Avtk::Masher( this, 5,36, 150, 126, "graph" );
  graph->clickMode( CLICK_TOGGLE );
  graph->rClickMode( RCLICK_NONE );
  
  dial1 = new Avtk::Dial( this,  8, 172, 45,45, "Time" );
  dial2 = new Avtk::Dial( this,110, 172, 45,45, "Vol" );
  dial3 = new Avtk::Dial( this, 60, 172, 45,45, "Pass" );
}

void MashaUI::widgetValueCB( Avtk::Widget* widget )
{
  float v = widget->value();
  //printf("Widget %s : %f\n", widget->label(), v );
  if( widget == dial1 )
  {
    graph->time = v;
    write_function( controller, MASHA_TIME, sizeof(float), 0, &v );
  }
  if( widget == dial2 )
  {
    graph->volume = v;
    write_function( controller, MASHA_AMP, sizeof(float), 0, &v );
  }
  if( widget == dial3 )
  {
    graph->passthrough = v;
    write_function( controller, MASHA_DRY_WET, sizeof(float), 0, &v );
  }
  if( widget == graph )
  {
    write_function( controller, MASHA_ACTIVE, sizeof(float), 0, &v );
  }
  redraw();
}

void MashaUI::lv2PortEvent( uint32_t index,
                            uint32_t buf_size,
                            uint32_t format,
                            const void* buffer )
{
  if( format != 0 )
    return;

  float v = *((float*)buffer);
  
  //printf("MashaUI port() %i : v\n", index, v );
  
  switch( index )
  {
  case MASHA_TIME:
    dial1->value( v );
    graph->time = v;
    break;
  case MASHA_AMP:
    dial2->value( v );
    graph->volume = v;
    break;
  case MASHA_DRY_WET:
    dial3->value( v );
    graph->passthrough = v;
    break;
  }
  redraw();
  return;
}


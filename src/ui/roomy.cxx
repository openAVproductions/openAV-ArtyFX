
#include "roomy.hxx"
#include "../dsp/roomy.hxx"

#include "common.hxx"
#include "../avtk/theme.hxx"
#include "graphs/reverb.hxx"
#include "headers/roomy.c"

RoomyUI::RoomyUI(PuglNativeWindow parent) :
  Avtk::UI( WIDTH, HEIGHT, parent, "Roomy (ArtyFX-OpenAV)" )
{
  Avtk::Image* i = new Avtk::Image( this, 0, 0, 160,  29, "header");
  i->load( roomy.pixel_data );
  
  rev   = new Avtk::Reverb( this, 5,36, 150, 126, "graph" );
  
  dial1 = new Avtk::Dial( this, 15, 172, 45,45, "Time" );
  dial2 = new Avtk::Dial( this, 55, 172, 45,45, "Damp" );
  dial3 = new Avtk::Dial( this, 95, 172, 45,45, "Dry Wet" );
}

void RoomyUI::widgetValueCB( Avtk::Widget* widget )
{
  float v = widget->value();
  printf("Widget %s : %f\n", widget->label(), v );
  if( widget == dial1 )
  {
    rev->size = v;
    write_function( controller, ROOMY_TIME, sizeof(float), 0, &v );
  }
  if( widget == dial2 )
  {
    rev->damping = v;
    write_function( controller, ROOMY_DAMPING, sizeof(float), 0, &v );
  }
  if( widget == dial3 )
  {
    rev->dryWet = v;
    write_function( controller, ROOMY_DRY_WET, sizeof(float), 0, &v );
  }
  redraw();
}

void RoomyUI::lv2PortEvent( uint32_t index,
                            uint32_t buf_size,
                            uint32_t format,
                            const void* buffer )
{
  if( format != 0 )
    return;

  float v = *((float*)buffer);
  
  //printf("RoomyUI port() %i : v\n", index, v );
  
  switch( index )
  {
  case ROOMY_TIME:
    dial1->value( v );
    rev->size = v;
    break;
  case ROOMY_DAMPING:
    dial2->value( v );
    rev->damping = v;
    break;
  case ROOMY_DRY_WET:
    dial3->value( v );
    rev->dryWet = v;
    break;
  }
  redraw();
  return;
}


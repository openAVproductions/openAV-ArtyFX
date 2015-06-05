
#include "driva.hxx"
#include "../dsp/driva.hxx"

#include "common.hxx"
#include "../avtk/theme.hxx"
#include "graphs/delay.hxx"
#include "headers/driva.c"

DrivaUI::DrivaUI(PuglNativeWindow parent) :
  Avtk::UI( WIDTH, HEIGHT, parent, "Driva (ArtyFX-OpenAV)" )
{
  Avtk::Image* i = new Avtk::Image( this, 0, 0, 160,  29, "header");
  i->load( driva.pixel_data );
  
  rev   = new Avtk::Delay( this, 5,36, 150, 126, "graph" );
  
  dial1 = new Avtk::Dial( this,  8, 172, 45,45, "Tone");
  dial2 = new Avtk::Dial( this, 60, 172, 45,45, "Amount" );
}

void DrivaUI::widgetValueCB( Avtk::Widget* widget )
{
  float v = widget->value();
  if( widget == dial1 )
  {
    //rev->feedback = v;
    write_function( controller, DRIVA_TONE, sizeof(float), 0, &v );
  }
  if( widget == dial2 )
  {
    //rev->volume = v;
    write_function( controller, DRIVA_AMOUNT, sizeof(float), 0, &v );
  }
  redraw();
}

void DrivaUI::lv2PortEvent( uint32_t index,
                            uint32_t buf_size,
                            uint32_t format,
                            const void* buffer )
{
  if( format != 0 )
    return;

  float v = *((float*)buffer);
  
  //printf("DrivaUI port() %i : v\n", index, v );
  
  switch( index )
  {
  case DRIVA_TONE:
    dial1->value( v );
    //rev->feedback = v;
    break;
  case DRIVA_AMOUNT:
    dial2->value( v );
//    rev->volume = v;
    break;
  }
  redraw();
  return;
}


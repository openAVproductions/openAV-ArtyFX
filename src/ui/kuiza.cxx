
#include "kuiza.hxx"
#include "../dsp/kuiza.hxx"

#include "common.hxx"
#include "../avtk/theme.hxx"
#include "graphs/delay.hxx"
#include "headers/kuiza.c"

KuizaUI::KuizaUI(PuglNativeWindow parent) :
  Avtk::UI( WIDTH, HEIGHT, parent, "Kuiza (ArtyFX-OpenAV)" )
{
  Avtk::Image* i = new Avtk::Image( this, 0, 0, 160,  29, "header");
  i->load( kuiza.pixel_data );
  
  rev   = new Avtk::Delay( this, 5,36, 150, 126, "graph" );
  
  dial1 = new Avtk::Dial( this,  8, 172, 45,45, "Low" );
  dial2 = new Avtk::Dial( this, 60, 172, 45,45, "Mid Low" );
  dial3 = new Avtk::Dial( this,110, 172, 45,45, "Mid High" );
  dial4 = new Avtk::Dial( this,110, 172, 45,45, "High" );
  dial5 = new Avtk::Dial( this,110, 172, 45,45, "Gain" );
}

void KuizaUI::widgetValueCB( Avtk::Widget* widget )
{
  float v = widget->value();
  //printf("Widget %s : %f\n", widget->label(), v );
  if( widget == dial1 )
  {
    rev->feedback = v;
    write_function( controller, KUIZA_GAIN_S1, sizeof(float), 0, &v );
  }
  if( widget == dial2 )
  {
    rev->volume = v;
    write_function( controller, KUIZA_GAIN_S2, sizeof(float), 0, &v );
  }
  if( widget == dial3 )
  {
    rev->time = v;
    write_function( controller, KUIZA_GAIN_S3, sizeof(float), 0, &v );
  }
  if( widget == dial4 )
  {
    //rev->time = v;
    write_function( controller, KUIZA_GAIN_S4, sizeof(float), 0, &v );
  }
  if( widget == dial5 )
  {
    //rev->time = v;
    write_function( controller, KUIZA_GAIN, sizeof(float), 0, &v );
  }
  redraw();
}

void KuizaUI::lv2PortEvent( uint32_t index,
                            uint32_t buf_size,
                            uint32_t format,
                            const void* buffer )

{
  if( format != 0 )
    return;

  float v = *((float*)buffer);
  
  //printf("KuizaUI port() %i : v\n", index, v );
  
  switch( index )
  {
  case KUIZA_GAIN_S1:
    dial1->value( v );
    rev->feedback = v;
    break;
  case KUIZA_GAIN_S2:
    dial2->value( v );
    rev->volume = v;
    break;
  case KUIZA_GAIN_S3:
    dial3->value( v );
    break;
  case KUIZA_GAIN_S4:
    dial4->value( v );
    break;
  case KUIZA_GAIN:
    dial5->value( v );
    break;
  }
  redraw();
  return;
}


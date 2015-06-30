
#include "driva.hxx"
#include "../dsp/driva.hxx"

#include "common.hxx"
#include "../avtk/theme.hxx"
#include "graphs/distortion.hxx"
#include "headers/driva.c"

DrivaUI::DrivaUI(PuglNativeWindow parent) :
  Avtk::UI( WIDTH, HEIGHT, parent, "Driva (ArtyFX-OpenAV)" )
{
  Avtk::Image* i = new Avtk::Image( this, 0, 0, 160,  29, "header");
  i->load( driva.pixel_data );
  
  graph = new Avtk::Distortion( this, 5,36, 150, 126, "graph" );
  
  //dial1 = new Avtk::Dial( this,  8, 172, 45,45, "Tone");
  dial2 = new Avtk::Dial( this, 60, 172, 45,45, "Amount" );
  
  tone = new Avtk::Button( this, 5, 172, 60, 30, "Tone" );
  dialog = new Avtk::Dialog( this, 0, 36, 160, 100, "Dialog" );
}

void DrivaUI::widgetValueCB( Avtk::Widget* widget )
{
  float v = widget->value();
  if( widget == tone )
  {
    dialog->run("Select Tone:","Test 1", Avtk::Dialog::OK_CANCEL );
  }
  if( widget == dialog )
  {
    // TODO : get the selected tone here
    //write_function( controller, DRIVA_TONE, sizeof(float), 0, &v );
    tone->value( 0 );
  }
  if( widget == dial2 )
  {
    graph->value( v );
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
    //dial1->value( v );
    // TODO: map from value -> label here
    //graph->value( v );
    break;
  case DRIVA_AMOUNT:
    dial2->value( v );
    graph->value( v );
    break;
  }
  redraw();
  return;
}


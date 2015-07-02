
#include "bitta.hxx"
#include "../dsp/bitta.hxx"

#include "common.hxx"
#include "../avtk/theme.hxx"
#include "graphs/bitcrusher.hxx"
#include "headers/bitta.c"

BittaUI::BittaUI(PuglNativeWindow parent) :
  Avtk::UI( WIDTH, HEIGHT, parent, "Della (ArtyFX-OpenAV)" )
{
  Avtk::Image* i = new Avtk::Image( this, 0, 0, 160,  29, "header");
  i->load( bitta.pixel_data );
  
  graph   = new Avtk::Bitcrusher( this, 5,36, 150, 126, "graph" );
  dial1 = new Avtk::Dial( this,  60, 170, 45,45, "Crush" );

  dial1->defaultValue( 0.5 );
}

void BittaUI::widgetValueCB( Avtk::Widget* widget )
{
  float v = widget->value();
  //printf("Widget %s : %f\n", widget->label(), v );
  if( widget == dial1 )
  {
    graph->value( v );
    write_function( controller, BITTA_CRUSH, sizeof(float), 0, &v );
  }
  redraw();
}

void BittaUI::lv2PortEvent( uint32_t index,
                            uint32_t buf_size,
                            uint32_t format,
                            const void* buffer )
{
  if( format != 0 )
    return;

  float v = *((float*)buffer);
  
  //printf("BittaUI port() %i : v\n", index, v );
  
  switch( index )
  {
  case BITTA_CRUSH:
    dial1->value( v );
    graph->value( v );
    break;
  case BITTA_DRYWET:
    // pass
    break;
  case BITTA_ACTIVE:
    // passthrough 
    break;
  }
  redraw();
  return;
}


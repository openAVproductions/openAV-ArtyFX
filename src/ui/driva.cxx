
#include "driva.hxx"
#include "../dsp/driva.hxx"

#include "common.hxx"
#include "../avtk/theme.hxx"
#include "graphs/distortion.hxx"
#include "headers/driva.c"

static const char* toneNames[] = {
  "Odie",
  "Grunge",
  "Distort",
  "Ratty",
  "Classic",
  "Morbid",
  "Metal"
};

DrivaUI::DrivaUI(PuglNativeWindow parent) :
  Avtk::UI( WIDTH, HEIGHT, parent, "Driva (ArtyFX-OpenAV)" )
{
  Avtk::Image* i = new Avtk::Image( this, 0, 0, 160,  29, "header");
  i->load( driva.pixel_data );
  
  for(int i = 0; i < 7; i++) {
   tones[i] = new Avtk::Button( this, 13, 36 + 22*i, 135, 18,
                                 toneNames[i] );
  }
  cancel = new Avtk::Button( this, 13, 36 + 22*7 + 6, 135, 20, "Cancel");
  cancel->visible(0);
  
  // Name shows up as first distortion model
  graph = new Avtk::Distortion( this, 5,36, 150, 126, "Odie" );
  tone = new Avtk::Button( this, 24, 176, 60, 30, "Tone" );
  dial2 = new Avtk::Dial( this, 98, 168, 45,45, "Amount" );
  
  show_tones( false );
}

void DrivaUI::show_tones(bool s)
{
    for( int i = 0; i < 7; i++) {
      tones[i]->visible( s );
      tones[i]->value( false );
    }
    cancel->visible( s );

    graph->visible( !s );
    dial2->visible( !s );
    tone->visible( !s );
    tone->value( false );
}

void DrivaUI::widgetValueCB( Avtk::Widget* widget )
{
  float v = widget->value();
  if( widget == tone )
  {
    show_tones( true );
  }
  if( widget == dial2 )
  {
    graph->value( v );
    write_function( controller, DRIVA_AMOUNT, sizeof(float), 0, &v );
  }
  if( widget == graph )
  {
    dial2->value( v );
  }
  if( widget == cancel )
  {
    show_tones( false );
  }

  for(int i = 0; i < 7; i++)
  {
    if( widget == tones[i] ) {
      float t = i;
      graph->label( toneNames[i] );
      write_function( controller, DRIVA_TONE, sizeof(float), 0, &t );
      show_tones( false );
      break;
    }
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
  int i = 0;

  switch( index )
  {
  case DRIVA_TONE:
    i = int(v);
    graph->label( toneNames[i] );
    break;
  case DRIVA_AMOUNT:
    dial2->value( v );
    graph->value( v );
    break;
  }
  redraw();
  return;
}


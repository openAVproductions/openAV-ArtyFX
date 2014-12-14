
#include "waveform.hxx"

#include <stdio.h>
#include <math.h>
#include "ui.hxx"
#include "theme.hxx"


using namespace Avtk;

Waveform::Waveform( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ ),
  waveformCr( 0 ),
  waveformSurf( 0 ),
  audioData( 0 ),
  zoom_( 1.0f )
{
  waveformSurf= cairo_image_surface_create ( CAIRO_FORMAT_ARGB32, w, h);
  waveformCr  = cairo_create ( waveformSurf );
  
  if( !waveformCr || !waveformSurf )
  {
    printf("Avtk::Waveform() error, waveformCr || waveformSurf == 0!\n");
    return;
  }
  
  std::vector<float> tmp;
  
  for(int i = 0; i < 4096; i++ )
    tmp.push_back( sin( i / float(20) ) );
  
  show( tmp );
  newWaveform = true;
}

void Waveform::show( std::vector<float> data )
{
  if ( data.size() == 0 )
  {
    return;
  }
  audioData = new std::vector<float>(data);
}

void Waveform::zoom( float zl )
{
  zoom_ = zl;
  newWaveform = true;
  ui->redraw();
}

void Waveform::zoomOffset( float po )
{
  zoomOffset_ = po;
  newWaveform = true;
  ui->redraw();
}

void Waveform::draw( cairo_t* cr )
{
  cairo_save( cr );
  
  roundedBox(cr, x, y, w, h, ui->theme->cornerRadius_ );
  ui->theme->color( cr, BG_DARK );
  cairo_fill_preserve(cr);
  ui->theme->color( cr, FG );
  cairo_set_line_width(cr, 1.4);
  cairo_stroke(cr);
  
  // check for a new waveform
  if( newWaveform )
  {
    cairo_rectangle(waveformCr, 0, 0, w, h);
    ui->theme->color( waveformCr, BG_DARK );
    cairo_fill( waveformCr );
    
    if ( !audioData )
    {
      // draw X
      cairo_move_to( waveformCr,  0, 0 );
      cairo_line_to( waveformCr,  w, h );
      cairo_move_to( waveformCr,  0, h );
      cairo_line_to( waveformCr,  w, 0 );
      cairo_set_source_rgba( waveformCr,  66 / 255.f,  66 / 255.f ,  66 / 255.f , 0.5 );
      cairo_stroke(waveformCr);
      
      // draw text
      cairo_move_to( waveformCr,  (w/2.f) - 65, (h/2.f) + 10 );
      cairo_set_source_rgb ( waveformCr, 0.6,0.6,0.6);
      cairo_set_font_size( waveformCr, 20 );
      cairo_show_text( waveformCr, "Load Sample" );
    }
    else
    {
      // find how many samples per pixel
      int samplesPerPix = audioData->size() / w;
      
      float withZoomSPP = samplesPerPix / zoom_;
      
      const int totalShownSamples = withZoomSPP * w;
      const int sampleOffset = (audioData->size() - totalShownSamples - 1) * zoomOffset_;
      
      //printf("sampsPerPx %i, with zoom %i\n",samplesPerPix, withZoomSPP);
      
      cairo_move_to( waveformCr, 0, (h/2) - ( audioData->at(0) * (h/2.2f) )  );
      
      cairo_set_line_join( cr, CAIRO_LINE_JOIN_ROUND);
      cairo_set_source_rgb( waveformCr, 1,1,1 );
      
      // loop over each pixel value we need
      for( int p = 0; p < w; p++ )
      {
        float average = 0.f;
        
        // calc value for this pixel
        for( int i = 0; i < withZoomSPP; i++ )
        {
          float tmp = audioData->at( sampleOffset + i + (p * withZoomSPP) );
          /*
          if ( tmp < 0 )
            tmp = -tmp;
          */
          average += tmp;
        }
        average = (average / withZoomSPP);
        
        //cairo_move_to( waveformCr, p, (h/2) - (averageL * (h/2.2f) )  );
        cairo_line_to( waveformCr, p, h/2.f - average*(h-40)/2.f );
        
        if( p % 128 == 0 )
        {
          // stroke the waveform
          cairo_stroke( waveformCr );
          cairo_move_to( waveformCr, p, h/2.f - average*(h-40)/2.f  );
        }
      }
      
      // stroke the remaining waveform lines
      cairo_stroke( waveformCr );
      
      // stroke a white line for the zoomOffset "center"
      // FIXME: not accurate
      int zoomOffsetAudioSample = audioData->size() * zoomOffset_;
      int zoomOffsetPixel = zoomOffsetAudioSample - sampleOffset;
      
      cairo_move_to( waveformCr, zoomOffsetPixel, 0 );
      cairo_line_to( waveformCr, zoomOffsetPixel, h );
      ui->theme->color( waveformCr, HIGHLIGHT, 0.8 );
      cairo_stroke( waveformCr );
    }
    newWaveform = false;
  }
  
  // paint cached waveform to normal cr
  cairo_set_source_surface(cr, waveformSurf, x, y);
  cairo_rectangle( cr, x, y, w, h);
  cairo_paint(cr);
  
  
  cairo_set_line_join( cr, CAIRO_LINE_JOIN_ROUND);
  //ui->theme->color( cr, HIGHLIGHT, 0.2 );
  //cairo_fill_preserve( cr );
  ui->theme->color( cr, BG, 0.8 );
  cairo_set_line_width(cr, 2.0);
  cairo_stroke(cr);
  
  cairo_restore( cr );
}


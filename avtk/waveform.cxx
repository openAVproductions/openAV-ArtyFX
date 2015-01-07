
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
  zoom_( 1.0f ),
  zoomOffset_(0),
  startPoint(0)
{
  waveformSurf= cairo_image_surface_create ( CAIRO_FORMAT_ARGB32, w_, h_);
  waveformCr  = cairo_create ( waveformSurf );
  
  if( !waveformCr || !waveformSurf )
  {
    printf("Avtk::Waveform() error, waveformCr || waveformSurf == 0!\n");
    return;
  }
  
  std::vector<float> tmp;
  
  for(int i = 0; i < 4096; i++ )
  {
    tmp.push_back( 0 ); // flat line
    //tmp.push_back( sin( i / float(20) ) ); // sine wave demo
  }
  show( tmp );
  newWaveform = true;
}

void Waveform::setStartPoint( float strt )
{
  startPoint = strt;
  ui->redraw( this );
}

void Waveform::show( long samps, const float* data )
{
  audioData.clear();
  
  for(int i = 0; i < samps; i++ )
  {
    audioData.push_back( *data++ );
  }
  newWaveform = true;
  ui->redraw();
}

void Waveform::show( std::vector<float> data )
{
  if ( data.size() == 0 )
  {
    printf("Waveform::show() data size == 0\n");
    return;
  }
  audioData.swap( data );
  newWaveform = true;
  ui->redraw();
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
  
  // check for a new waveform
  if( newWaveform )
  {
    cairo_rectangle(waveformCr, 0, 0, w_, h_);
    theme_->color( waveformCr, BG_DARK );
    cairo_fill( waveformCr );
    
    if ( false )
    {
      // draw X
      cairo_move_to( waveformCr,  0, 0 );
      cairo_line_to( waveformCr,  w_, h_ );
      cairo_move_to( waveformCr,  0, h_ );
      cairo_line_to( waveformCr,  w_, 0 );
      cairo_set_source_rgba( waveformCr,  66 / 255.f,  66 / 255.f ,  66 / 255.f , 0.5 );
      cairo_stroke(waveformCr);
      
      // draw text
      cairo_move_to( waveformCr,  (w_/2.f) - 65, (h_/2.f) + 10 );
      cairo_set_source_rgb ( waveformCr, 0.6,0.6,0.6);
      cairo_set_font_size( waveformCr, 20 );
      cairo_show_text( waveformCr, "Load Sample" );
    }
    else
    {
      // find how many samples per pixel
      int samplesPerPix = audioData.size() / w_;
      
      float withZoomSPP = samplesPerPix / zoom_;
      
      const int totalShownSamples = withZoomSPP * w_;
      const int sampleOffset = (audioData.size() - totalShownSamples - 1) * zoomOffset_;
      
      //printf("sampsPerPx %i, with zoom %i\n",samplesPerPix, withZoomSPP);
      
      cairo_move_to( waveformCr, 0, (h_/2) - ( audioData.at(0) * (h_/2.2f) )  );
      
      cairo_set_line_join( cr, CAIRO_LINE_JOIN_ROUND);
      cairo_set_source_rgb( waveformCr, 1,1,1 );
      cairo_set_line_width(cr, theme_->lineWidthThin() );
      
      // loop over each pixel value we need
      for( int p = 0; p < w_; p++ )
      {
        float average = 0.f;
        
        // calc value for this pixel
        for( int i = 0; i < withZoomSPP; i++ )
        {
          float tmp = audioData.at( sampleOffset + i + (p * withZoomSPP) );
          /*
          if ( tmp < 0 )
            tmp = -tmp;
          */
          average += tmp;
        }
        average = (average / withZoomSPP);
        
        //cairo_move_to( waveformCr, p, (h/2) - (averageL * (h/2.2f) )  );
        cairo_line_to( waveformCr, p, h_/2.f - average*(h_-40)/2.f );
        
        if( p % 128 == 0 )
        {
          // stroke the waveform, necessary for good anti-aliasing
          cairo_stroke( waveformCr );
          cairo_move_to( waveformCr, p, h_/2.f - average*(h_-40)/2.f  );
        }
      }
      
      // stroke the remaining waveform lines
      cairo_stroke( waveformCr );
      
      /*
      // stroke a white line for the zoomOffset "center"
      // FIXME: not accurate
      int zoomOffsetAudioSample = audioData->size() * zoomOffset_;
      int zoomOffsetPixel = zoomOffsetAudioSample - sampleOffset;
      
      cairo_move_to( waveformCr, zoomOffsetPixel, 0 );
      cairo_line_to( waveformCr, zoomOffsetPixel, h );
      theme_->color( waveformCr, HIGHLIGHT, 0.8 );
      cairo_stroke( waveformCr );
      */
    }
    newWaveform = false;
  }
  
  
  // paint cached waveform to normal cr
  cairo_set_source_surface(cr, waveformSurf, x_, y_);
  cairo_rectangle( cr, x_, y_, w_, h_);
  cairo_paint(cr);
  cairo_stroke( cr );
  
  // new path, drawing start line
  cairo_new_sub_path( cr );
  cairo_move_to( cr, x_ + startPoint * w_, y_ + 0  );
  cairo_line_to( cr, x_ + startPoint * w_, y_ + h_ );
  theme_->color( cr, HIGHLIGHT );
  cairo_set_line_width(cr, theme_->lineWidthWide() );
  cairo_stroke( cr );
  
  cairo_new_sub_path( cr );
  //theme_->color( cr, BG );
  cairo_rectangle( cr, x_, y_, w_, h_);
  cairo_set_line_join( cr, CAIRO_LINE_JOIN_ROUND);
  theme_->color( cr, FG );
  cairo_set_line_width(cr, theme_->lineWidthNorm() );
  cairo_stroke(cr);
  
  cairo_restore( cr );
}


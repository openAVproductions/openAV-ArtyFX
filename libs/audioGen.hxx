
#ifndef OPENAV_AVTK_AUDIOGEN_HXX
#define OPENAV_AVTK_AUDIOGEN_HXX

#include "pffft.hxx"
#include "plotter.hxx"
#include <math.h>
#include <stdio.h>

#define N 1024
#define SAMPLE_RATE 44100

float binToFreq( int sr, int fftSize, int index )
{
  if( index >= 0 && index < fftSize / 2 )
    return float(index) / fftSize * sr;
  
  // not valid bin : either below 0 or above Nyquist
  return -1;
}


void sawtooth(int count, float* array, float freq, int harmonics)
{
  for(float i = 0.f; i < count; i++ )
  {
    float wavelength = SAMPLE_RATE / freq;
    
    float out = 0.f;
    
    // loop over each harmonic
    for(int h = 1; h < harmonics + 1; h++ )
    {
      float harmonicFreq = h * freq;
      
      if ( harmonicFreq < SAMPLE_RATE / 2.f )
      {
        // formula for sawtooth wave:
        //out += sin( h * (i/wavelength) ) / h;
        
        // formula for square
        out += sin( (2*h-1) * (i/wavelength) ) / (2*h-1);
      }
      else
      {
        break;
      }
    }
    
    // write the output value of the added harmonics to the array
    array[int(i)] = out;
  }
}


void windowBlackmanHarris( int size, float* data )
{
  const float M = size-1;
  
  for( int i = 0; i < size; i++)
  {
    data[i] = 0.42 - (0.5 * cos(2.0*M_PI*(float)i/M)) + (0.08*cos(4.0*M_PI*(float)i/M));
  }
}

void audioDataSpectrum( int size, float* data )
{
  bool cplx = false;
  
  if ( size < N )
  {
    printf("%s : Size < FFT Radix, error: abort & return\n", __PRETTY_FUNCTION__ );
    return;
  }
  // PFFFT setup code
  int Nfloat  = (cplx ? N*2 : N);
  int Nbytes  = Nfloat * sizeof(float);
  float *in   = (float*)pffft_aligned_malloc(Nbytes);
  float *out  = (float*)pffft_aligned_malloc(Nbytes);
  float *work = (float*)pffft_aligned_malloc(Nbytes);
  
  float window[N];
  windowBlackmanHarris( N, window );
  Plotter::plot( "window", N, window );
  
  // write band-limited saw wave into array
  sawtooth( N, in, 440, 500 );
  
  {
    PFFFT_Setup *s = pffft_new_setup(N, cplx ? PFFFT_COMPLEX : PFFFT_REAL);
    if (s)
    {
      // multiply in the window
      for(int i = 0; i < N; i++ )
      {
        in[i] *= window[i];
      }
      
      // pffft_transform
      pffft_transform_ordered(s, in, out, work, PFFFT_FORWARD);
      
      for(int i = 0; i < N / 2; i++ )
      {
        // the the frequency of bin
        float freq = binToFreq( 44100, N, i );
        
        float re = out[i*2  ]*out[i*2  ];
        float im = out[i*2+1]*out[i*2+1];
        
        work[i] = sqrt( re + im );
        
        /*
        if( i < 20 )
        {
          printf( "freq %f : %f\n", freq, work[i] );
        }
        */
        
        data[i] = work[i];
      }
      
      Plotter::plot( "output", 200, work );
      
      //pffft_transform(s, X, Z, Y, PFFFT_BACKWARD);
      
      pffft_destroy_setup(s); 
      
      //flops = (max_iter*2) * ((cplx ? 5 : 2.5)*N*log((double)N)/M_LN2); // see http://www.fftw.org/speed/method.html
      //show_output("PFFFT", N, cplx, flops, t0, t1, max_iter);
    }
  }
  
  
}

#endif 


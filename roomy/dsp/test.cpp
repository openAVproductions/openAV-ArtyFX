
#include "dsp_reverb.hxx"

#define SIZE 1024

#include <string.h>

int main()
{
  Reverb d(44100);
  
  float audio[SIZE];
  float* tmp[2];
  tmp[0] = &audio[0];
  tmp[1] = &audio[0];
  
  memset( audio, 0, SIZE * sizeof(float) );
  
  for(int i = 0; i < 100; i++)
    d.process(SIZE, tmp, tmp );
  
  d.rt60( 0.67 );
  
  for(int i = 0; i < 100; i++)
    d.process(SIZE, tmp, tmp );
  
  return 0;
}

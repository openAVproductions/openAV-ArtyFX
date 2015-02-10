
#include "dsp_wah.hxx"

#define SIZE 1024

#include <string.h>

int main()
{
  Wah d(44100);
  
  float audio[SIZE];
  
  memset( audio, 0, SIZE * sizeof(float) );
  
  for(int i = 0; i < 100; i++)
    d.process(SIZE, &audio[0], &audio[0] );
  
  d.setFreq( 0.67 );
  
  for(int i = 0; i < 100; i++)
    d.process(SIZE, &audio[0], &audio[0] );
  
  return 0;
}

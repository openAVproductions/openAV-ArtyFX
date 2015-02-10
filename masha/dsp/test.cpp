
#include "dsp_masher.hxx"

#define SIZE 2048

#include <string.h>

int main()
{
  Masher d(44100);
  
  float audio[SIZE];
  
  memset( audio, 0, SIZE * sizeof(float) );
  
  for(int i = 0; i < 100; i++)
    d.process(SIZE, &audio[0], &audio[0] );
  
  //d.setBPM( 1 );
  d.duration( 0.67 );
  
  for(int i = 0; i < 100; i++)
    d.process(SIZE, &audio[0], &audio[0] );
  
  return 0;
}

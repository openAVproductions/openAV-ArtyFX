
#include "dsp_delay.hxx"

#define SIZE 2048

int main()
{
  Delay d(44100);
  
  float audio[SIZE];
  
  memset( audio, 0, SIZE * sizeof(float) );
  
  for(int i = 0; i < 100; i++)
    d.process(SIZE, &audio[0], &audio[0] );
  
  d.setBPM( 1 );
  d.setValue( 0.67 );
  
  for(int i = 0; i < 100; i++)
    d.process(SIZE, &audio[0], &audio[0] );
  
  return 0;
}

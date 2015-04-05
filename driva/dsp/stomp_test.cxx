
/** Stompbox Test
 * This test file serves the purpose of testing preset switching and
 * such, to ensure it doesn't click / pop when doing so.
 */


#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "rr/StompBox.h"
#include "plotter.h"

int main()
{
  int rate = 44100;
  StompBox* s = new StompBox( rate );
  
  const int size = 512;
  
  float* audioIn  = (float*) calloc( size, sizeof(float) );
  float* audioOut = (float*) calloc( size, sizeof(float) );
  
  for(int i = 0; i < size; i++)
    audioIn[i] = sinf( i * 6.28f / 44100.f );
  
  s->setpreset( 2 );
  
  s->process( size/2, audioIn, audioOut );
  
  s->setpreset( 1 );
  
  {
    float tmpIn [size];
    float tmpOut[size];
    
    memcpy( tmpIn ,  &audioIn[size/2], sizeof(float)*size/2 );
    memcpy( tmpOut, &audioOut[size/2], sizeof(float)*size/2 );
    
    s->process( size/2, tmpIn, tmpOut );
  }
  
  
  /*
  {
    float tmp[size];
    memcpy( tmp, audioIn, sizeof(float)*size );
    //s->process( size, &tmp[0], &tmp[0] );
    //s->process( size/2, &audioIn[size/2], &tmp[0] );
  }
  */
  
  // unmute 
  printf("Stompbox1: unmute after vol %d\n", s->getpar( 0 ) );
  
  /*
  int vol = s->getpar( 0 );
  s->changepar( 0, 0 );
  //s->process( size, &audioIn[0], &audioOut[0] );
  
  s->changepar( 0, vol );
  */
  
  //s->process( size, &audioIn[0], &audioOut[0] );
  s->process( size/2, &audioIn[size/2], &audioOut[size/2] );
  
  
  plotter_write( "in.dat",  size, audioIn  );
  plotter_write( "out.dat", size, audioOut );
  
  return 0;
  
}

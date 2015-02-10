
#include "shared.hxx"

#define SIZE 128

#include <string.h>

int main()
{
  Ducka d(44100);
  
  float ports[SIZE];
  float audio[SIZE];
  
  memset( audio, 0, SIZE * sizeof(float) );
  
  Ducka::connect_port( &d, 0, &audio[0] );
  Ducka::connect_port( &d, 0, &audio[0] );
  Ducka::connect_port( &d, 0, &audio[0] );
  Ducka::connect_port( &d, 0, &audio[0] );
  Ducka::connect_port( &d, 0, &audio[0] );
  Ducka::connect_port( &d, 0, &audio[1] );
  Ducka::connect_port( &d, 0, &audio[2] );
  Ducka::connect_port( &d, 0, &audio[3] );
  Ducka::connect_port( &d, 0, &audio[4] );
  // Atom
  Ducka::connect_port( &d, 0, &audio[5] );
  
  
  Ducka::run( &d, SIZE );
  
  return 0;
}

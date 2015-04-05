
#ifndef OPENAV_DSP_PLOTTER_H
#define OPENAV_DSP_PLOTTER_H

#include <stdio.h>
#include <unistd.h>

inline static void plotter_write( const char* name, size_t frames, float* data )
{
  FILE* fp = fopen( name, "w");
  size_t i = 0;
  while( --frames )
  {
    fprintf(fp, "%f\n", data[i++] );
  }
  fclose(fp);
}

#endif /* OPENAV_DSP_PLOTTER_H */


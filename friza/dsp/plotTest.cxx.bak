
#include "dsp_plotter.hxx"

int main()
{
  Plotter p;
  
  const int size = 102;
  
  float* d = new float[size];
  
  for( int i = 0; i < size; i++ )
  {
    if ( i % 17 == 0 )
    {
      d[i] = 2;
    }
    else
      d[i] = i / 1023.f;
  }
  
  p.plot( size, d );
  
  //p.showLast();
  
  return 0;
}

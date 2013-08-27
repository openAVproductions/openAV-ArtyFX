

#ifndef OPENAV_DSP_MASHER_H
#define OPENAV_DSP_MASHER_H

#include <math.h>
#include <cmath>
#include <cstdlib>

/** Masher
 * This class mashes a stream of audio by keeping a history, and re-looping the
 * previous X frames when the effect is enabled.
**/
class Masher // : Effect
{
  public:
    Masher(int sr) :
      sampleRate ( sr ),
      history( new float[ sr ] ),
      framesPerBeat(22050)
    {
    }
    
    void bpm(int b)
    {
      framesPerBeat = sampleRate / b * 60;
    }
    
    int getNumInputs() { return 1; }
    int getNumOutputs(){ return 1; }
    
    void process (int count, float** input, float** output)
    {
      
    }
  
  private:
    const int sampleRate;
    const float* history;
    int framesPerBeat;
};

#endif // OPENAV_DSP_REVERB_H

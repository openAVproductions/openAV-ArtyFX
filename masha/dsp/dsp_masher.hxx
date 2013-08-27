

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
    
    void amplitude(float a)
    {
      _amplitude = a;
    }
    void duration(float d)
    {
      _duration = d;
    }
    void dryWet(float d)
    {
      _dryWet = d;
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
    
    float _amplitude;
    float _duration;
    float _dryWet;
};

#endif // OPENAV_DSP_REVERB_H



#ifndef OPENAV_DSP_MASHER_H
#define OPENAV_DSP_MASHER_H

#include <math.h>
#include <cmath>
#include <cstdlib>
#include <iostream>

/** Masher
 * This class mashes a stream of audio by keeping a history, and re-looping the
 * previous X frames when the effect is enabled.
**/
class Masher // : Effect
{
  public:
    Masher(int sr) :
      sampleRate ( sr ),
      history( new float[ sr * 2 ] ),
      framesPerBeat(22050),
      _active(false)
    {
      _playhead   = 0;
      _recordHead = sr * 2;
    }
    
    void active(bool a)
    {
      _active = a;
      //printf("active() int %i\n", _active );
      if ( _active )
      {
        _playhead = 0;
      }
      else
      {
        _recordHead = 0;
      }
      
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
      _dryWet = 1-d;
    }
    
    void bpm(int b)
    {
      framesPerBeat = sampleRate / b * 60;
    }
    
    int getNumInputs() { return 1; }
    int getNumOutputs(){ return 1; }
    
    void process (int count, float** input, float** output)
    {
      if ( _active )
      {
        // playback
        int newDuration = int( _duration * 4.9f);
        float beats = 0.f;
        if ( newDuration == 0 )
          beats = 1;
        if ( newDuration == 1 )
          beats = 2;
        if ( newDuration == 2 )
          beats = 4;
        if ( newDuration == 3 )
          beats = 8;
        if ( newDuration == 4 )
          beats = 16;
        
        int loopFrames = (framesPerBeat * beats) / 16;
        
        printf("playback, loopframes = %i, _playhead %f\n", loopFrames, _playhead);
        
        for(int i = 0; i < count; i++ )
        {
          if ( _playhead >= loopFrames )
          {
            _playhead = 0;
            printf("playback, resetting _playhead\n");
          }
          
          output[0][i] = input[0][i] * _dryWet + _amplitude * history[ _recordHead + int(_playhead) ];
          _playhead++;
        }
      }
      else
      {
        std::cout << "Record " << _recordHead << std::endl;
        // record *backwards*, so playing is forwards trough the buffer
        for(int i = 0; i < count; i++ )
        {
          if ( _recordHead < 0 )
          {
            _recordHead = sampleRate * 2;
          }
          
          history[ _recordHead-- ] = input[0][i];
          
          output[0][i] = input[0][i];
        }
      }
      
    }
  
  private:
    const int sampleRate;
    float* history;
    int framesPerBeat;
    
    float _playhead;
    long _recordHead;
    
    int _active;
    
    float _amplitude;
    float _duration;
    float _dryWet;
};

#endif // OPENAV_DSP_REVERB_H

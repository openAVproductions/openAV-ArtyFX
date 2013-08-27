

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
      activ(false)
    {
      playhead   = 0;
      recordHead = sr * 2;
    }
    
    void active(bool a)
    {
      activ = a;
      //printf("active() int %i\n", active );
      if ( activ )
      {
        playhead = 0;
      }
      else
      {
        recordHead = sampleRate * 2;
      }
      
    }
    void amplitude(float a)
    {
      amp = a;
    }
    void duration(float d)
    {
      durat = d;
    }
    void dryWet(float d)
    {
      dryWe = 1-d;
    }
    
    void bpm(int b)
    {
      framesPerBeat = sampleRate / b * 60;
    }
    
    int getNumInputs() { return 1; }
    int getNumOutputs(){ return 1; }
    
    void process (int count, float** input, float** output)
    {
      if ( activ )
      {
        // playback
        int newDuration = int( durat * 4.9f);
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
        
        printf("playback, loopframes = %i, playhead %i\n", loopFrames, playhead);
        
        for(int i = 0; i < count; i++ )
        {
          /*
          if ( playhead >= loopFrames )
          {
            playhead = 0;
            printf("playback, resetting playhead\n");
          }
          */
          
          output[0][i] = input[0][i] * dryWe + amp * history[ (recordHead + playhead ) % sampleRate * 2 ];
          playhead++;
          
          printf("playback, loopframes = %i, playhead %i\n", loopFrames, playhead);
        }
      }
      else
      {
        std::cout << "Record " << recordHead << std::endl;
        // record *backwards*, so playing is forwards trough the buffer
        for(int i = 0; i < count; i++ )
        {
          if ( recordHead < 0 )
          {
            recordHead = sampleRate * 2;
          }
          
          history[ recordHead ] = input[0][i];
          
          recordHead = recordHead - 1;
          
          output[0][i] = input[0][i];
        }
      }
      
    }
  
  private:
    const int sampleRate;
    float* history;
    int framesPerBeat;
    
    int playhead;
    long recordHead;
    
    int activ;
    
    float amp;
    float durat;
    float dryWe;
};

#endif // OPENAV_DSP_REVERB_H

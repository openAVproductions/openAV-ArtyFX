/*
 * Author: Harry van Haaren 2013
 *         harryhaaren@gmail.com
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef OPENAV_DSP_MASHER_H
#define OPENAV_DSP_MASHER_H

#include <math.h>
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

#define BUFFER_SIZE 44100*2

/** Masher
 * This class mashes a stream of audio by keeping a history, and re-looping the
 * previous X frames when the effect is enabled.
**/
class Masher // : Effect
{
  public:
    Masher(int sr) :
      sampleRate ( sr ),
      framesPerBar(22050),
      _activated(false)
    {
      history = (float*)malloc( sizeof(float) * BUFFER_SIZE ),
      
      _recording = false;
      playhead   = 0;
      recordHead = sr * 2;
    }
    
    void active(bool a)
    {
      if ( _activated == false && a == true )
      {
        playhead = 0;
        recordHead = 0;
        _recording = true;
      }
      if ( _activated == true && a == false )
      {
        _activated = false;
        _recording = false;
      }
      
      _activated = a;
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
      framesPerBar = sampleRate / b * 60;
    }
    
    int getNumInputs() { return 1; }
    int getNumOutputs(){ return 1; }
    
    void process (long count, float* input, float* output)
    {
      float beats = 0.f;
      int newDuration = int( durat * 4.9f);
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
      long smashSize = (beats * framesPerBar) / 8;
      
      for(long i = 0; i < count; i++ )
      {
        float tmp = 0.f;
        if ( _recording )
        {
          if ( recordHead >= (16 * framesPerBar) / 8 )
          {
            _recording = false;
          }
          
          history[ recordHead++ ] = input[i];
        }
        
        if ( recordHead > smashSize ) // then playback
        {
          if ( playhead >= smashSize )
          {
            playhead = 0;
          }
          tmp = history[ playhead++ ];
        }
        
        if ( !_activated )
        {
          tmp = 0;
          dryWe = 0;
        }
        
        output[i] = tmp * amp + input[i] * (1-dryWe);
      }
      
    }
  
  private:
    const int sampleRate;
    float* history;
    long framesPerBar;
    
    long playhead;
    long recordHead;
    
    bool _activated;
    bool _recording;
    
    float amp;
    float durat;
    float dryWe;
};

#endif // OPENAV_DSP_REVERB_H

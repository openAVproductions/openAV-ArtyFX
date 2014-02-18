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

// 

#ifndef OPENAV_DSP_DELAY_H
#define OPENAV_DSP_DELAY_H

#include <cstring>
#include <cmath>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>

#define DB_CO(g)   ((g) > -90.0f ? powf(10.0f, (g) * 0.05f) : 0.0f)

using namespace std;

/** Delay
 *  A delay effect implementation.
**/
class Delay // : Effect
{
  public:
    Delay(int sr) :
      samplerate( sr )
    {
      _active = true;
      
      writeHead = 0;
      
      delayVolume = 0.7;
      
      delayTimeSamps = sr / 2;
      
      // allocate 1 second max buffer length
      buffer = new float[ sr ];
    }
    
    float getValue()
    {
      return delayVolume;
    }
    
    void setValue(float v)
    {
      if ( v < 0.f ) v = 0.f;
      if ( v > 1.f ) v = 1.f;
      
      // FIXME: get BPM & use BPM to calculate delay time
      //int bpm = 120;
      int delTimeQuantized = int(v * 3.8f);
      
      switch( delTimeQuantized )
      {
        case 0:
          delayTimeSamps = samplerate * 0.25;
          break;
        case 1:
          delayTimeSamps = samplerate * 0.5;
          break;
        case 2:
          delayTimeSamps = samplerate * 1;
          break;
      }
    }
    
    void setVolume( float v )
    {
      delayVolume = v;
    }
    float getVolume()
    {
      return delayVolume;
    }
    
    void active(bool a)
    {
      _active = a;
    }
    
    int getNumInputs() { return 1; }
    int getNumOutputs(){ return 1; }
    
    void process (long count, float* input, float* output)
    {
      // set input -> output
      memcpy( output, input, count * sizeof(float) );
      
      if ( _active )
      {
        // add delayed signal
        for (int i = 0; i < count; i++)
        {
          if ( writeHead > delayTimeSamps )
            writeHead = 0;
          
          int readPos = writeHead - delayTimeSamps;
          if ( readPos < 0 )
            readPos += delayTimeSamps;
          
          output[i] += buffer[readPos];
          
          buffer[writeHead] = input[i];
          writeHead++;
        }
      }
      else
      {
        // not active, so forward writeHead with 0.f's?
      }
      
    }
  
  private:
    int samplerate;
    bool _active;
    
    int bpm;
    int delayTimeSamps;
    
    float* buffer;
    int writeHead;
    
    int timeUnit;
    
    float delayVolume;
};

#endif // OPENAV_DSP_DELAY_H
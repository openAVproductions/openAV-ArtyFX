/*
 * Author: Harry van Haaren 2013
 *         harryhaaren@gmail.com
 * 
 * DSP adapted from SWH Barry's Satan Maximizer, 
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

#ifndef OPENAV_DSP_DISTORTION_H
#define OPENAV_DSP_DISTORTION_H

#include <cstring>
#include <cmath>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>

#define BUFFER_SIZE 16
#define BUFFER_MASK 15

#define DB_CO(g)   ((g) > -90.0f ? powf(10.0f, (g) * 0.05f) : 0.0f)

using namespace std;

/** Distortion
 *  A hard working distortion maximizer, removes dynamic-range, and introduces
 *  distortion harmonics.
**/
class Distortion // : Effect
{
  public:
    Distortion(int sr)
    {
      _active = false;
      
      // find default values that work in general
      env_time_p = 0.5;
      knee_point = 0.1;
      
      env = 0.0f;
      buffer = new float[BUFFER_SIZE];
      buffer_pos = 0;
    }
    
    float getValue()
    {
      return knee_point;
    }
    
    float getEnvelope()
    {
      return env;
    }
    
    void setEnvelope( float e )
    {
      if ( e < 0.f ) e = 0.f;
      if ( e > 1.f ) e = 1.f;
      
      // invert value: a shorter envelope results in harsher distortion, so
      // turning "up" the dial makes it more distorted / high frequency content.
      env_time_p = 1-e;
    }
    
    void setValue(float v)
    {
      if ( v < 0.f ) v = 0.f;
      if ( v > 1.f ) v = 1.f;
      knee_point = v;
    }
    
    void active(bool a)
    {
      _active = a;
      
      env = 0.0f;
      memset(buffer, 0, sizeof(float) * BUFFER_SIZE);
      buffer_pos = 0;
      
      // uncomment to re-calc frequency values on enable / disable
      // commented because the LV2 calls setValue() on *every* process()
      //setValue( value );
    }
    
    int getNumInputs() { return 1; }
    int getNumOutputs(){ return 1; }
    
    void process (long count, float* input, float* output)
    {
      if ( !_active )
      {
        // pass audio trough from in -> out
        memcpy( output, input, count * sizeof(float) );
        return;
      }
      
      long pos;
      int delay;
      float env_tr, env_sc, knee;
      float env_time = 2 + (env_time_p * 28.f);
      
      if (env_time < 2.0f)
      {
              env_time = 2.0f;
      }
      
      float kneeRanged = 0 - (knee_point*45);
      knee = DB_CO( kneeRanged );
      delay = roundf(env_time * 0.5f);
      env_tr = 1.0f / env_time;
      
      for (pos = 0; pos < count; pos++)
      {
        if (fabs(input[pos]) > env)
        {
          env = fabs(input[pos]);
        }
        else
        {
          env = fabs(input[pos]) * env_tr + env * (1.0f - env_tr);
        }
        
        if (env <= knee)
        {
          env_sc = 1.0f / knee;
        }
        else
        {
          env_sc = 1.0f / env;
        }
        
        buffer[buffer_pos] = input[pos];
        float volReduction = 0.1 + pow( 1- 0.9*knee_point, 4);
        output[pos] = (buffer[(buffer_pos - delay) & BUFFER_MASK] * env_sc ) * volReduction;
        buffer_pos = (buffer_pos + 1) & BUFFER_MASK;
      }
      
      env = env;
      buffer_pos = buffer_pos;
    }
  
  private:
    bool _active;
    
    float env;
    float* buffer;
    int   buffer_pos;
    
    float env_time_p;
    float knee_point;
};

#endif // OPENAV_DSP_DISTORTION_H

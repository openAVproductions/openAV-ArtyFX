/*
 * Author: Harry van Haaren 2014
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

#ifndef OPENAV_DSP_BITCRUSHER_H
#define OPENAV_DSP_BITCRUSHER_H

#include <cstring>
#include <cmath>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>

using namespace std;

/** BitCrusher
 *  Bit-crushing and sample rate reduction.
**/
class BitCrusher // : Effect
{
  public:
    BitCrusher(int sr) :
      samplerate( sr )
    {
      _active = true;
      
      bits = 16;
      rate = 1;
      
      m = 1 << (bits-1);
      count = 0;
    }
    
    float getValue()
    {
      return bits;
    }
    
    void setValue(float v)
    {
      if ( v < 0.f ) v = 0.f;
      if ( v > 1.f ) v = 1.f;
      
      m = 1 + (1 << ((int) (((1-v)*15))) );
      
      //m = 1 << ( int(v*16.f) -1 );
    }
    
    void active(bool a)
    {
      _active = a;
    }
    
    int getNumInputs() { return 1; }
    int getNumOutputs(){ return 1; }
    
    void process (long count, float* in, float* out)
    {
      if ( _active )
      {
        for(int i = 0; i < count; i++)
        {
          float tmp = 0.f;
          
          count+=rate;
          
          if ( count >= 1)
          {
            count -= 1;
            
            tmp=(long int)( *in++ * m)/(float)m;
          }
          *out++ = tmp;
        }

      }
      else
      {
        // set input -> output
        memcpy( out, in, count * sizeof(float) );
      }
    }
  
  private:
    int samplerate;
    bool _active;
    
    int bits;
    float rate;
    
    long int m;
    float count;
};

#endif // OPENAV_DSP_BITCRUSHER_H

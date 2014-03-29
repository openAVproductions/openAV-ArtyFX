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

#ifndef OPENAV_DSP_WIDENER_H
#define OPENAV_DSP_WIDENER_H

#include <cstring>
#include <cmath>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>

using namespace std;

/** Widener
 *  A stereo width effect, which implements a mid-side width control, and
 *  right channel invert option.
**/
class Widener // : Effect
{
  public:
    Widener(int sr) :
      samplerate( sr )
    {
      _active = true;
      
      width = 0.5;
      invertRight = false;
      
    }
    
    float getValue()
    {
      return width;
    }
    
    void setValue(float v)
    {
      if ( v < 0.f ) v = 0.f;
      if ( v > 1.f ) v = 1.f;
      
      width = v;
    }
    
    void setInvert( bool i )
    {
      invertRight = i;
    }
    float getInvert()
    {
      return invertRight;
    }
    
    void active(bool a)
    {
      _active = a;
    }
    
    int getNumInputs() { return 2; }
    int getNumOutputs(){ return 2; }
    
    void process (long count, float* inL, float* inR, float* outL, float* outR)
    {
      if ( _active )
      {
        // do mid-side width calculations
        float tmp = 1 / max( 1 + width , 2.f );
        float mid  = 1     * tmp;
        float side = width * tmp;
        
        for (int i = 0; i < count; i++)
        {
          // apply mid / side coefficents
          float m = ( *inL + *inR ) * mid;
          float s = ( *inL - *inR ) * side;
          
          // write output
          *outL = m - s;
          *outR = m + s;
          
          // move buffer pointers to next sample
          inL++;
          inR++;
          outL++;
          outR++;
        }
      }
      else
      {
        // set input -> output
        memcpy( outL, inL, count * sizeof(float) );
        memcpy( outR, inR, count * sizeof(float) );
      }
    }
  
  private:
    int samplerate;
    bool _active;
    
    float width;
    bool  invertRight;
};

#endif // OPENAV_DSP_WIDENER_H

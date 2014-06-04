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

#ifndef OPENAV_DSP_COMPANDER_H
#define OPENAV_DSP_COMPANDER_H

#include <cstring>
#include <cmath>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>

using namespace std;

/** Compander
 *  Bit-crushing and sample rate reduction.
**/
class Compander // : Effect
{
  public:
    Compander(int sr) :
      samplerate( sr )
    {
      _active = true;
      
      init();
    }
    
    float getValue()
    {
      return factor;
    }
    
    void setValue(float v)
    {
      if ( v < 0.f ) v = 0.f;
      if ( v > 1.f ) v = 1.f;
      
      factor = 1 + v * 7;
    }
    
    void setThreshold( float v )
    {
      if ( v < 0.f ) v = 0.f;
      if ( v > 1.f ) v = 1.f;
      
      // -20 to -80dB range
      threshold = (v * 70) - 80;
    }
    
    void setRelease( float v)
    {
      if ( v < 0.f ) v = 0.f;
      if ( v > 1.f ) v = 1.f;
      release = v * 980 + 20;
    }
    
    
    void active(bool a)
    {
      _active = a;
    }
    
    int getNumInputs() { return 1; }
    int getNumOutputs(){ return 1; }
    
    void process (long count, float* input0, float* output0)
    {
      if ( _active )
      {
        float 	fSlow0 = (0.0010000000000000009f * attack);
        float 	fSlow1 = (0.0010000000000000009f * release);
        float 	fSlow2 = (0.0010000000000000009f * factor);
        float 	fSlow3 = (0.0010000000000000009f * threshold);
        
        for (int i=0; i<count; i++)
        {
          fRec1[0] = (fSlow0 + (0.999f * fRec1[1]));
          float fTemp0 = expf((0 - (fConst0 / fRec1[0])));
          float fTemp1 = (float)input0[i];
          fVec0[0] = fTemp1;
          fRec6[0] = (fVec0[0] + (0.161758f * fRec6[2]));
          fRec5[0] = (((0.733029f * fRec5[2]) + (0.161758f * fRec6[0])) - fRec6[2]);
          fRec4[0] = (((0.94535f * fRec4[2]) + (0.733029f * fRec5[0])) - fRec5[2]);
          fRec3[0] = (((0.990598f * fRec3[2]) + (0.94535f * fRec4[0])) - fRec4[2]);
          float fTemp2 = ((0.990598f * fRec3[0]) - fRec3[2]);
          fRec10[0] = ((0.479401f * fRec10[2]) + fVec0[1]);
          fRec9[0] = (((0.876218f * fRec9[2]) + (0.479401f * fRec10[0])) - fRec10[2]);
          fRec8[0] = (((0.976599f * fRec8[2]) + (0.876218f * fRec9[0])) - fRec9[2]);
          fRec7[0] = (((0.9975f * fRec7[2]) + (0.976599f * fRec8[0])) - fRec8[2]);
          float fTemp3 = ((0.9975f * fRec7[0]) - fRec7[2]);
          
          //float fTemp4 = fabsf(min((float)100, max(1e-05f, sqrtf( (faustpower<2>(fTemp3) + faustpower<2>(fTemp2))))));
          float fTemp4 = fabsf(min((float)100, max(1e-05f, sqrtf( ( pow( fTemp3, 2 ) + pow( fTemp2, 2 ) ) ) ) ) );
          
          fRec11[0] = (fSlow1 + (0.999f * fRec11[1]));
          float fTemp5 = expf((0 - (fConst0 / fRec11[0])));
          fRec2[0] = (((1.0f - fTemp5) * fTemp4) + (fTemp5 * max(fTemp4, fRec2[1])));
          fRec0[0] = ((fRec2[0] * (1.0f - fTemp0)) + (fTemp0 * fRec0[1]));
          fRec12[0] = (fSlow2 + (0.999f * fRec12[1]));
          fRec13[0] = (fSlow3 + (0.999f * fRec13[1]));
          float fTemp6 = powf(10,(0.05f * fRec13[0]));
          output0[i] = (float)(0.7071067811865476f * ((powf(min((float)1, max(1e-07f, sinf((1.5707963267948966f * (fRec12[0] * min((1.0f / fRec12[0]), fRec0[0])))))),(logf(fTemp6) / logf(sinf((1.5707963267948966f * (fRec12[0] * fTemp6)))))) * (fTemp3 + fTemp2)) / fRec0[0]));
          // post processing
          fRec13[1] = fRec13[0];
          fRec12[1] = fRec12[0];
          fRec0[1] = fRec0[0];
          fRec2[1] = fRec2[0];
          fRec11[1] = fRec11[0];
          fRec7[2] = fRec7[1]; fRec7[1] = fRec7[0];
          fRec8[2] = fRec8[1]; fRec8[1] = fRec8[0];
          fRec9[2] = fRec9[1]; fRec9[1] = fRec9[0];
          fRec10[2] = fRec10[1]; fRec10[1] = fRec10[0];
          fRec3[2] = fRec3[1]; fRec3[1] = fRec3[0];
          fRec4[2] = fRec4[1]; fRec4[1] = fRec4[0];
          fRec5[2] = fRec5[1]; fRec5[1] = fRec5[0];
          fRec6[2] = fRec6[1]; fRec6[1] = fRec6[0];
          fVec0[1] = fVec0[0];
          fRec1[1] = fRec1[0];
        }
      }
      else
      {
        // set input -> output
        memcpy( output0, input0, count * sizeof(float) );
      }
    }
  
  private:
    int samplerate;
    bool _active;
    
    float attack;
    float release;
    float factor;
    float fRec1[2];
    float fConst0;
    float fVec0[2];
    float fRec6[3];
    float fRec5[3];
    float fRec4[3];
    float fRec3[3];
    float fRec10[3];
    float fRec9[3];
    float fRec8[3];
    float fRec7[3];
    float fRec11[2];
    float fRec2[2];
    float fRec0[2];
    
    float fRec12[2];
    float threshold;
    float fRec13[2];
  
    void init()
    {
      attack = 1.0f;
      for (int i=0; i<2; i++) fRec1[i] = 0;
      fConst0 = (1e+03f / float(min(192000, max(1, samplerate))));
      for (int i=0; i<2; i++) fVec0[i] = 0;
      for (int i=0; i<3; i++) fRec6[i] = 0;
      for (int i=0; i<3; i++) fRec5[i] = 0;
      for (int i=0; i<3; i++) fRec4[i] = 0;
      for (int i=0; i<3; i++) fRec3[i] = 0;
      for (int i=0; i<3; i++) fRec10[i] = 0;
      for (int i=0; i<3; i++) fRec9[i] = 0;
      for (int i=0; i<3; i++) fRec8[i] = 0;
      for (int i=0; i<3; i++) fRec7[i] = 0;
      release = 2e+01f;
      for (int i=0; i<2; i++) fRec11[i] = 0;
      for (int i=0; i<2; i++) fRec2[i] = 0;
      for (int i=0; i<2; i++) fRec0[i] = 0;
      factor = 1.0f;
      for (int i=0; i<2; i++) fRec12[i] = 0;
      threshold = -4e+01f;
      for (int i=0; i<2; i++) fRec13[i] = 0;
    }
  
};

#endif // OPENAV_DSP_COMPANDER_H

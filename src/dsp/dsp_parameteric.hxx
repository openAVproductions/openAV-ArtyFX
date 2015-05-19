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

// 

#ifndef OPENAV_DSP_PARAMETERIC_H
#define OPENAV_DSP_PARAMETERIC_H

#include <cstring>
#include <cmath>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>

using namespace std;

// Fons' Parametric filter
#include "eq/filters.h"

/** Parameteric Equalizer
 *  A parameteric equalizer based on the parameteric DSP code by Fons Adriaensen
 *  
**/
class Parameteric
{
  public:
    // convienience enum to access ports
    enum {
      GAIN    =  3,
      S1_GAIN =  7,
      S2_GAIN = 11,
      S3_GAIN = 15,
      S4_GAIN = 19,
    };
    
    void setGain( int selection, float gain )
    {
      switch( selection )
      {
        case 0: controls[GAIN]    = gain; break;
        case 1: controls[S1_GAIN] = gain; break;
        case 2: controls[S2_GAIN] = gain; break;
        case 3: controls[S3_GAIN] = gain; break;
        case 4: controls[S4_GAIN] = gain; break;
        default: break;
      }
    }
    
    Parameteric(int sr) :
      samplerate( sr )
    {
      filter = new Ladspa_Paramfilt( sr );
      
      _active = true;
      filter->active( _active );
      
      controls[0] = 0;   // audio I/O: init to 0x0 for now
      controls[1] = 0;   // as we don't know the buffer addresses yet
      
      controls[2] = true;   // filter enable
      controls[3] = 0.f;    // filter master gain
      
      controls[4] = true;   // section 1 enable
      controls[5] = 55.f;   // section 1 freq
      controls[6] = 1.2f;   // section 1 band
      controls[7] = 0.f;    // section 1 gain
      
      controls[8] = true;   // section 2 enable
      controls[9] = 220.f;  // section 2 freq
      controls[10] = 1.f;   // section 2 band
      controls[11] = 0.f;   // section 2 gain
      
      controls[12] = true;  // section 3 enable
      controls[13] = 1760.f;// section 3 freq
      controls[14] = 1.f;   // section 3 band
      controls[15] = 0.f;   // section 3 gain
      
      controls[16] = true;  // section 4 enable
      controls[17] = 7040.f;// section 4 freq
      controls[18] = 1.6f;  // section 4 band
      controls[19] = 0.f;   // section 4 gain
      
      for(int i = 0; i < 20; i++)
      {
        filter->setport( i, &controls[i] );
      }
      
      // intialize the filter co-effients
      filter->active(true);
    }
    
    void active(bool a)
    {
      if ( a != _active )
      {
        //printf("active = %i\n", int(a) );
        _active = a;
      }
    }
    
    int getNumInputs() { return 1; }
    int getNumOutputs(){ return 1; }
    
    void process (long count, float* input, float* output)
    {
      if ( !_active )
      {
        if( input != output )
          memcpy( output, input, sizeof(float)*count );
        return;
      }
      
      // connect audio buffers
      filter->setport( 0, input );
      filter->setport( 1, output );
      
      // run filter, not adding
      filter->runproc( count, false );
    }
    
    ~Parameteric()
    {
      delete filter;
    }
  
  private:
    
    
    int samplerate;
    bool _active;
    
    float controls[20];
    
    Ladspa_Paramfilt *filter;
};

#endif // OPENAV_DSP_PARAMETERIC_H

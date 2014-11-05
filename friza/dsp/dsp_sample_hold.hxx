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

/** DSP : SampleHoldShift
 * This plugin is a variation of the standard sample-and-hold. The main
 * difference is that after the initial loop has been created, it continues to
 * sample, and gradually moves the loop position (not lenght) along.
 * 
 * The inspiration for this plugin arised from when Gian of the MOD project showed
 * OpenAV an existing sample-and-hold plugin, and we got brainstorming on possible
 * improvements to give it a more dynamic and creative use-case.
 */

#ifndef OPENAV_DSP_SAMPLE_HOLD_H
#define OPENAV_DSP_SAMPLE_HOLD_H

#include <cmath>
#include <cstring>
#include <cstdio>

#define TESTING 1

#include "dsp_plotter.hxx"

class SampleHoldShift
{
  public:
    SampleHoldShift( int s ) :
      sr ( s ),
      recordHead( 0 ),
      playHead( 0 ),
      xfade( 512 ),
      
      buffer( new float[sr] )
    {
      _length = 2048;

#ifdef TESTING
      printf("testing\n");
      // for testing, we can put a sin-wave into the buffer, and test playback
      // for glitches / with it. 
      for( int i = 0; i < _length; i++ )
        buffer[i] = sin( i * 3.1415 * 100 );
      
      Plotter::plot( _length, buffer );
#endif // TESTING
    }
    
    /// set to true when the effect should take place
    void doIt( bool d )
    {
      // triggers the recording of the next ~second of audio
      recordHead = 0;
      playHead = 0;
      
      _doIt = d;
    }
    
    /// set the lenght of the sample-and-hold loop
    void length( float l )
    {
      _length = 2048; //64 + 1024 * 4 * l;
    }
    
    /// playback volume of the effect
    void volume( float v )
    {
      _volume = v;
    }
    
    /// position of the buffer that's currently playing back
    void position( float p )
    {
      _position = p;
    }
    
    void process (long nframes, float* input, float* output )
    {
#ifdef TESTING
#else
      // copy input material to buffer, unless done
      if ( recordHead + nframes < sr )
      {
        memcpy( &buffer[recordHead], input, sizeof(float) * nframes );
        recordHead += nframes;
      }
#endif // TESTING
      
      if ( input != output )
      {
        memcpy( output, input, sizeof(float) * nframes );
        memset( output,     0, sizeof(float) * nframes );
      }
      
      if ( _doIt )
      {
        /*
        // pre-mature optimization
        // no wrap needed
        if ( playHead + nframes < _length )
        {
          for(int i = 0; i < nframes; i++ )
          {
            output[i] = input[i] + buffer[playHead++];
          }
        }
        else
        */
        {
          for(int i = 0; i < nframes; i++ )
          {
            if ( playHead >= _length )
              playHead = 0;
            
            output[i] += buffer[playHead++] * _volume;
          }
        }
        
      }
    }
    
    ~SampleHoldShift()
    {
      delete buffer;
    }
  
  private:
    int sr;
    
    bool _doIt;
    long _length;
    float _volume;
    float _position;
    
    long recordHead;
    long playHead;
    
    int xfade;
    
    float* buffer;
};

#endif // OPENAV_DSP_SAMPLE_HOLD_H


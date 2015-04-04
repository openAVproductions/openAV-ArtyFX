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
#include "dsp_linearbrown.hxx"

class SampleHoldShift
{
  public:
    SampleHoldShift( int s ) :
      sr ( s ),
      _playForward( true ),
      _doIt ( 0 ),
      recordHead( 0 ),
      playHead( 0 ),
      xfade( 512 ),
      buffer( 0 )
    {
      buffer = new float[sr*5];
      _length = 2048;

#ifdef TESTING
      printf("testing\n");
      // for testing, we can put a sin-wave into the buffer, and test playback
      // for glitches / with it. 
      for( int i = 0; i < _length; i++ )
        buffer[i] = sin( i * 3.1415 * 40 );
      
      Plotter::plot( _length, buffer );
#endif // TESTING
    }
    
    /// set to true when the effect should take place
    void doIt( bool d )
    {
      if( d && !_doIt )
      {
        playHead = 0;
        _playForward = true;
        // set starting point of _position?
        //smoother.set_current( _position * (recordHead - nframes) );
      }
      if( !d && _doIt )
        recordHead = 0;
      
      _doIt = d;
    }
    
    /// set the lenght of the sample-and-hold loop
    void length( float l )
    {
      _length = 64 + 1024.f * 8.f * l;//2048; 
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
      
      /*
#ifdef TESTING
#else
      // copy input material to buffer, unless done
      if ( recordHead + nframes < sr )
      {
        memcpy( &buffer[recordHead], input, sizeof(float) * nframes );
        recordHead += nframes;
      }
#endif // TESTING
      */
      
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
          // calculate the start pos for playback of this nframes
          float ph = _position * _length;
          //float ph = _position * (recordHead - nframes - _length);
          int act = 0;
          
          int playOffset = 1;
          
          for(int i = 0; i < nframes; i++ )
          {
            smoother.step( ph );
            act = smoother.get();
            
            if( act >= 0 && act < recordHead )
            {
              output[i] = buffer[act + playHead];
              
              /*
              playHead += playOffset;
              
              if( playHead >= _length )
              {
                _playForward = !_playForward;
                playOffset = -1;
              }
              else if( playHead <= 0 )
              {
                _playForward = !_playForward;
                playOffset = 1;
              }
              */
            }
            
          }
          
          //printf("%i\n", act );
          
          
        }
        
      }
      else
      {
        // grab audio from input and record into ringbuffer
        if( recordHead + nframes >= sr * 5 )
        {
          recordHead = 0;
        }
        
        {
          for(int i = 0; i < nframes; i++ )
          {
            buffer[recordHead] = input[i];
            recordHead++;
          }
        }
        if ( input != output )
        {
          memcpy( output, input, sizeof(float) * nframes );
        }
        
        //memset( output, 0, sizeof(float)*nframes );
      }
    }
    
    ~SampleHoldShift()
    {
      delete[] buffer;
    }
  
  private:
    int sr;
    
    BrownLinearExpo smoother;
    
    bool _playForward;
    
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


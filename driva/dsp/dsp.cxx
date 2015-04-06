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

#include "shared.hxx"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "rr/StompBox.h"

LV2_Handle Driva::instantiate(const LV2_Descriptor* descriptor,
                              double samplerate,
                              const char* bundle_path,
                              const LV2_Feature* const* features)
{
  return (LV2_Handle) new Driva( samplerate );
}

Driva::Driva(int rate)
{
  audioInput = 0;
  audioOutput = 0;
  
  wave1type = 0;
  
  dspStompbox1 = new StompBox( rate );
  
  // init preset
  dspStompbox1->setpreset( 0 );
  wave1type = 0;
}

Driva::~Driva()
{
  delete dspStompbox1;
}

void Driva::activate(LV2_Handle instance)
{
}

void Driva::deactivate(LV2_Handle instance)
{
}

void Driva::connect_port(LV2_Handle instance, uint32_t port, void *data)
{
  Driva* self = (Driva*) instance;
  
  switch (port)
  {
      case INPUT:
          self->audioInput      = (float*)data;
          break;
      case OUTPUT:
          self->audioOutput     = (float*)data;
          break;
      
      case DRIVA_TONE:
          self->controlWave1type   = (float*)data;
          break;
      case DRIVA_AMOUNT:
          self->controlWave1amount = (float*)data;
          break;
  }
}

void Driva::run(LV2_Handle instance, uint32_t nframes)
{
  Driva* self = (Driva*) instance;
  
  /// audio inputs
  float* in  = self->audioInput;
  float* out = self->audioOutput;
  
  
  /// deal with preset switches
  if ( int(*self->controlWave1type) != self->wave1type )
  {
    
    self->wave1type = int(*self->controlWave1type);
    self->dspStompbox1->setpreset( self->wave1type );
    
    //print f("Stompbox1: to preset %f\n", *self->controlWave1type );
    
    // mute the effect - keeping presets volume
    int oldVol = self->dspStompbox1->getpar( 0 );
    self->dspStompbox1->changepar( 0, 0 );
    
    //print f(" changepar vol = %d\n", oldVol );
    
    float tmpIn [nframes];
    float tmpOut[nframes];
    
    memcpy( tmpIn ,  in, sizeof(float)*nframes );
    memcpy( tmpOut, out, sizeof(float)*nframes );
    
    self->dspStompbox1->process( nframes, tmpIn, tmpOut );
    
    /* unmute 
    print f("Stompbox1: unmute after preset %f : vol %d\n",
      *self->controlWave1type, oldVol );
    */
    self->dspStompbox1->changepar( 0, oldVol );
  }
  
  /// set "running variables": range in Stompbox is 0-127, so scale to that
  
  // gain
  self->dspStompbox1->changepar( 4, 7 + *self->controlWave1amount * 120 );
  
  self->dspStompbox1->process( nframes, in, out );
  
  // FIXME: nuke input to 0 again, due to waveshaper write-issue; possibly due
  // to inplace processing of Stompbox, which isn't cleared by JACK if nothing
  // is connected -> causes lots of noise on output.
  if( in != out )
  {
    memset( in, 0, sizeof(float) * nframes );
  }
}

void Driva::cleanup(LV2_Handle instance)
{
  delete ((Driva*) instance);
}

const void* Driva::extension_data(const char* uri)
{
    return NULL;
}

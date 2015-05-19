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

#include "filta.hxx"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

LV2_Handle Filta::instantiate(const LV2_Descriptor* descriptor,
                              double samplerate,
                              const char* bundle_path,
                              const LV2_Feature* const* features)
{
  return (LV2_Handle) new Filta( samplerate );
}

Filta::Filta(int rate) :
  dspFilters( rate )
{
  dspFilters.active( true );
}


void Filta::activate(LV2_Handle instance)
{
}

void Filta::deactivate(LV2_Handle instance)
{
}

void Filta::connect_port(LV2_Handle instance, uint32_t port, void *data)
{
  Filta* self = (Filta*) instance;
  
  switch (port)
  {
      case FILTA_INPUT_L:
          self->audioInputL    = (float*)data;
          break;
      case FILTA_INPUT_R:
          self->audioInputR    = (float*)data;
          break;
          break;
      case FILTA_OUTPUT_L:
          self->audioOutputL   = (float*)data;
          break;
      case FILTA_OUTPUT_R:
          self->audioOutputR   = (float*)data;
          break;
      
      case FILTA_FREQ_CONTROL:
          self->freqControl  = (float*)data;
          break;
      case FILTA_ACTIVE:
          self->activeControl  = (float*)data;
          break;
  }
}

void Filta::run(LV2_Handle instance, uint32_t n_samples)
{
  Filta* self = (Filta*) instance;
  
  /// audio inputs
  float* inL  = self->audioInputL;
  float* inR  = self->audioInputR;
  float* outL = self->audioOutputL;
  float* outR = self->audioOutputR;
  
  float* buf[4] = {
    inL, inR,
    outL, outR
  };
  
  /// control inputs
  float active = *self->activeControl;
  float freqControl = *self->freqControl;
  
  if ( active > 0.5 )
    self->dspFilters.active( true  );
  else
    self->dspFilters.active( false );
  
  self->dspFilters.setValue( freqControl );
  
  self->dspFilters.process( n_samples, &buf[0], &buf[2] );
}

void Filta::cleanup(LV2_Handle instance)
{
  delete ((Filta*) instance);
}

const void* Filta::extension_data(const char* uri)
{
    return NULL;
}

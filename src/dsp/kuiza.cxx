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

#include "kuiza.hxx"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dsp_parameteric.hxx"

LV2_Handle Kuiza::instantiate(const LV2_Descriptor* descriptor,
                              double samplerate,
                              const char* bundle_path,
                              const LV2_Feature* const* features)
{
  return (LV2_Handle) new Kuiza( samplerate );
}

Kuiza::Kuiza(int rate)
{
  filter = new Parameteric( rate );
}

void Kuiza::activate(LV2_Handle instance)
{
}

void Kuiza::deactivate(LV2_Handle instance)
{
}

void Kuiza::connect_port(LV2_Handle instance, uint32_t port, void *data)
{
  Kuiza* self = (Kuiza*) instance;
  
  switch (port)
  {
      case KUIZA_INPUT:
          self->audioInput    = (float*)data;
          break;
      case KUIZA_OUTPUT:
          self->audioOutput   = (float*)data;
          break;
      
      case KUIZA_GAIN:
          self->controlGain  = (float*)data;
          break;
      case KUIZA_GAIN_S1:
          self->controlS1Gain  = (float*)data;
          break;
      case KUIZA_GAIN_S2:
          self->controlS2Gain  = (float*)data;
          break;
      case KUIZA_GAIN_S3:
          self->controlS3Gain  = (float*)data;
          break;
      case KUIZA_GAIN_S4:
          self->controlS4Gain  = (float*)data;
          break;
      
      case KUIZA_ACTIVE:
          self->controlActive  = (float*)data;
          break;
  }
}

void Kuiza::run(LV2_Handle instance, uint32_t n_samples)
{
  Kuiza* self = (Kuiza*) instance;
  
  /// audio inputs
  float* in  = self->audioInput;
  float* out = self->audioOutput;
  
  /// control inputs
  float active = *self->controlActive;
  
  float gain  = *self->controlGain;
  // scale 0-1 to -10 - 10 db
  self->filter->setGain( 0, (gain * 20) - 10 );
  
  float gainS1  = *self->controlS1Gain;
  float gainS2  = *self->controlS2Gain;
  float gainS3  = *self->controlS3Gain;
  float gainS4  = *self->controlS4Gain;
  
  // scale
  self->filter->setGain( 1, (gainS1 * 40) -20 );
  self->filter->setGain( 2, (gainS2 * 40) -20 );
  self->filter->setGain( 3, (gainS3 * 40) -20 );
  self->filter->setGain( 4, (gainS4 * 40) -20 );
  
  if ( active > 0.5 )
    self->filter->active( true  );
  else
    self->filter->active( false );
  
  self->filter->process( n_samples, in, out );
}

void Kuiza::cleanup(LV2_Handle instance)
{
  delete ((Kuiza*)instance)->filter;
  
  delete ((Kuiza*) instance);
}

const void* Kuiza::extension_data(const char* uri)
{
    return NULL;
}

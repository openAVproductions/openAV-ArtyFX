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

#include "shared.hxx"

#include "dsp_sample_hold.hxx"

LV2_Handle Friza::instantiate(const LV2_Descriptor* descriptor,
                              double samplerate,
                              const char* bundle_path,
                              const LV2_Feature* const* features)
{
  return (LV2_Handle) new Friza( samplerate );
}

Friza::Friza(int rate)
{
  shs = new SampleHoldShift( rate );
  
  audioInput = 0;
  audioOutput = 0;
  
  length  = 0;
  doIt    = 0;
}

Friza::~Friza()
{
  delete shs;
}

void Friza::activate(LV2_Handle instance)
{
  Friza* self = (Friza*) instance;
}

void Friza::deactivate(LV2_Handle instance)
{
}

void Friza::connect_port(LV2_Handle instance, uint32_t port, void *data)
{
  Friza* self = (Friza*) instance;
  
  switch (port)
  {
      case FRIZA_INPUT:
          self->audioInput      = (float*)data;
          break;
      case FRIZA_OUTPUT:
          self->audioOutput     = (float*)data;
          break;
      
      case FRIZA_DO_IT:
          self->doIt   = (float*)data;
          break;
      case FRIZA_LENGTH:
          self->length = (float*)data;
          break;
      case FRIZA_POSITION:
          self->position = (float*)data;
          break;
      case FRIZA_VOLUME:
          self->volume = (float*)data;
          break;
  }
}

void Friza::run(LV2_Handle instance, uint32_t nframes)
{
  Friza* self = (Friza*) instance;
  
  /// audio inputs
  float* in  = self->audioInput;
  float* out = self->audioOutput;
  
  self->shs->length  ( *self->length   );
  self->shs->position( *self->position );
  self->shs->volume  ( *self->volume   );
  self->shs->doIt    ( (*self->doIt >= 0.500) ? true : false  );
  
  self->shs->process( nframes, in, out );
}

void Friza::cleanup(LV2_Handle instance)
{
  delete ((Friza*) instance);
}

const void* Friza::extension_data(const char* uri)
{
  return 0;
}

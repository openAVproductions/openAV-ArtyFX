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

#include "dsp_wah.hxx"

LV2_Handle Whaaa::instantiate(const LV2_Descriptor* descriptor,
                              double samplerate,
                              const char* bundle_path,
                              const LV2_Feature* const* features)
{
  return (LV2_Handle) new Whaaa( samplerate );
}

Whaaa::Whaaa(int rate)
{
  wah = new Wah( rate );
  
  audioInput = 0;
  audioOutput = 0;
  
  freq  = 0;
  range = 0;
  drive = 0;
  mix   = 0;
}

Whaaa::~Whaaa()
{
  delete wah;
}

void Whaaa::activate(LV2_Handle instance)
{
  Whaaa* self = (Whaaa*) instance;
  self->wah->init();
}

void Whaaa::deactivate(LV2_Handle instance)
{
}

void Whaaa::connect_port(LV2_Handle instance, uint32_t port, void *data)
{
  Whaaa* self = (Whaaa*) instance;
  
  switch (port)
  {
      case WHAAA_INPUT:
          self->audioInput      = (float*)data;
          break;
      case WHAAA_OUTPUT:
          self->audioOutput     = (float*)data;
          break;
      
      case WHAAA_FREQ:
          self->freq   = (float*)data;
          break;
      case WHAAA_RANGE:
          self->range = (float*)data;
          break;
      case WHAAA_DRIVE:
          self->drive = (float*)data;
          break;
      case WHAAA_MIX:
          self->mix = (float*)data;
          break;
  }
}

void Whaaa::run(LV2_Handle instance, uint32_t nframes)
{
  Whaaa* self = (Whaaa*) instance;
  
  /// audio inputs
  float* in  = self->audioInput;
  float* out = self->audioOutput;
  
  self->wah->setFreq ( *self->freq  );
  self->wah->setRange( *self->range );
  self->wah->setDrive( *self->drive );
  self->wah->setMix  ( *self->mix   );
  
  self->wah->process( nframes, in, out );
}

void Whaaa::cleanup(LV2_Handle instance)
{
  delete ((Whaaa*) instance);
}

const void* Whaaa::extension_data(const char* uri)
{
  return 0;
}

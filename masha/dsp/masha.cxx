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

#include "masha.hxx"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dsp_masher.hxx"

class Masha
{
  public:
    Masha(int rate);
    ~Masha(){}
    static LV2_Handle instantiate(const LV2_Descriptor* descriptor,
                                  double samplerate,
                                  const char* bundle_path,
                                  const LV2_Feature* const* features);
    static void activate(LV2_Handle instance);
    static void deactivate(LV2_Handle instance);
    static void connect_port(LV2_Handle instance, uint32_t port, void *data);
    static void run(LV2_Handle instance, uint32_t n_samples);
    static void cleanup(LV2_Handle instance);
    static const void* extension_data(const char* uri);
    
    /// audio buffers
    float* audioInputL;
    float* audioInputR;
    float* audioOutputL;
    float* audioOutputR;
    
    /// control signals
    float* controlTime;
    float* controlAmp;
    float* controlDryWet;
    float* controlActive;
    
  private:
    Masher* dspMasherL;
    Masher* dspMasherR;
};


static const LV2_Descriptor descriptor =
{
  MASHA_URI,
  Masha::instantiate,
  Masha::connect_port,
  Masha::activate,
  Masha::run,
  Masha::deactivate,
  Masha::cleanup,
  Masha::extension_data
};


LV2_SYMBOL_EXPORT const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
  if (index == 0) return &descriptor;
  else return NULL;
}


LV2_Handle Masha::instantiate(const LV2_Descriptor* descriptor,
                              double samplerate,
                              const char* bundle_path,
                              const LV2_Feature* const* features)
{
  return (LV2_Handle) new Masha( samplerate );
}

Masha::Masha(int rate)
{
  dspMasherL = new Masher( rate );
  dspMasherR = new Masher( rate );
}


void Masha::activate(LV2_Handle instance)
{
}

void Masha::deactivate(LV2_Handle instance)
{
}

void Masha::connect_port(LV2_Handle instance, uint32_t port, void *data)
{
  Masha* self = (Masha*) instance;
  
  switch (port)
  {
      case MASHA_INPUT_L:
          self->audioInputL    = (float*)data;
          break;
      case MASHA_INPUT_R:
          self->audioInputR    = (float*)data;
          break;
          break;
      case MASHA_OUTPUT_L:
          self->audioOutputL   = (float*)data;
          break;
      case MASHA_OUTPUT_R:
          self->audioOutputR   = (float*)data;
          break;
      
      case MASHA_TIME:
          self->controlTime    = (float*)data;
          break;
      case MASHA_AMP:
          self->controlAmp     = (float*)data;
          break;
      case MASHA_DRY_WET:
          self->controlDryWet  = (float*)data;
          break;
      
      case MASHA_ACTIVE:
          self->controlActive  = (float*)data;
          break;
  }
}

void Masha::run(LV2_Handle instance, uint32_t n_samples)
{
  Masha* self = (Masha*) instance;
  
  /// audio inputs
  float* inL  = self->audioInputL;
  float* inR  = self->audioInputR;
  float* outL = self->audioOutputL;
  float* outR = self->audioOutputR;
  
  /// control inputs
  float time   = *self->controlTime;
  float amp    = *self->controlAmp;
  float dryWet = *self->controlDryWet;
  
  float active = *self->controlActive;
  
  self->dspMasherL->amplitude( amp    );
  self->dspMasherL->duration ( time   );
  self->dspMasherL->dryWet   ( dryWet );
  self->dspMasherL->active   ( active );
  
  self->dspMasherR->amplitude( amp    );
  self->dspMasherR->duration ( time   );
  self->dspMasherR->dryWet   ( dryWet );
  self->dspMasherR->active   ( active );
  
  self->dspMasherL->process( n_samples, inL, outL );
  self->dspMasherR->process( n_samples, inR, outR );
}

void Masha::cleanup(LV2_Handle instance)
{
  delete ((Masha*) instance);
}

const void* Masha::extension_data(const char* uri)
{
    return NULL;
}

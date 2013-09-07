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

#include "roomy.hxx"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dsp_reverb.hxx"

class Roomy
{
  public:
    Roomy(int rate);
    ~Roomy(){}
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
    float* controlDamping;
    float* controlDryWet;
    
  private:
    Reverb dspReverb;
};


static const LV2_Descriptor descriptor =
{
  ROOMY_URI,
  Roomy::instantiate,
  Roomy::connect_port,
  Roomy::activate,
  Roomy::run,
  Roomy::deactivate,
  Roomy::cleanup,
  Roomy::extension_data
};


LV2_SYMBOL_EXPORT const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
  if (index == 0) return &descriptor;
  else return NULL;
}


LV2_Handle Roomy::instantiate(const LV2_Descriptor* descriptor,
                              double samplerate,
                              const char* bundle_path,
                              const LV2_Feature* const* features)
{
  return (LV2_Handle) new Roomy( samplerate );
}

Roomy::Roomy(int rate) :
  dspReverb( rate )
{
}


void Roomy::activate(LV2_Handle instance)
{
}

void Roomy::deactivate(LV2_Handle instance)
{
}

void Roomy::connect_port(LV2_Handle instance, uint32_t port, void *data)
{
  Roomy* self = (Roomy*) instance;
  
  switch (port)
  {
      case ROOMY_INPUT_L:
          self->audioInputL    = (float*)data;
          break;
      case ROOMY_INPUT_R:
          self->audioInputR    = (float*)data;
          break;
          break;
      case ROOMY_OUTPUT_L:
          self->audioOutputL   = (float*)data;
          break;
      case ROOMY_OUTPUT_R:
          self->audioOutputR   = (float*)data;
          break;
      
      case ROOMY_TIME:
          self->controlTime    = (float*)data;
          break;
      case ROOMY_DAMPING:
          self->controlDamping = (float*)data;
          break;
      
      case ROOMY_DRY_WET:
          self->controlDryWet  = (float*)data;
          break;
  }
}

void Roomy::run(LV2_Handle instance, uint32_t n_samples)
{
  Roomy* self = (Roomy*) instance;
  
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
  float time    = *self->controlTime;
  float damping = *self->controlDamping;
  float dryWet  = *self->controlDryWet;
  
  self->dspReverb.rt60    ( time    );
  self->dspReverb.damping ( damping );
  self->dspReverb.dryWet  ( dryWet  );
  
  self->dspReverb.process( n_samples, &buf[0], &buf[2] );
}

void Roomy::cleanup(LV2_Handle instance)
{
  delete ((Roomy*) instance);
}

const void* Roomy::extension_data(const char* uri)
{
    return NULL;
}

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

#include "dsp_delay.hxx"

class Della
{
  public:
    Della(int rate);
    ~Della(){}
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
    float* audioInput;
    float* audioOutput;
    
    /// control signals
    float* controlDelay;
    float* controlVolume;
    float* controlFeedback;
    float* controlActive;
    
  private:
    /// runtime variables
    bool active;
    Delay* delay;
};


static const LV2_Descriptor descriptor =
{
  DELLA_URI,
  Della::instantiate,
  Della::connect_port,
  Della::activate,
  Della::run,
  Della::deactivate,
  Della::cleanup,
  Della::extension_data
};


LV2_SYMBOL_EXPORT const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
  if (index == 0) return &descriptor;
  else return NULL;
}


LV2_Handle Della::instantiate(const LV2_Descriptor* descriptor,
                              double samplerate,
                              const char* bundle_path,
                              const LV2_Feature* const* features)
{
  return (LV2_Handle) new Della( samplerate );
}

Della::Della(int rate)
{
  delay = new Delay( rate );
}

void Della::activate(LV2_Handle instance)
{
}

void Della::deactivate(LV2_Handle instance)
{
}

void Della::connect_port(LV2_Handle instance, uint32_t port, void *data)
{
  Della* self = (Della*) instance;
  
  switch (port)
  {
      case DELLA_INPUT:
          self->audioInput    = (float*)data;
          break;
      case DELLA_OUTPUT:
          self->audioOutput   = (float*)data;
          break;
      
      case DELLA_TIME:
          self->controlDelay  = (float*)data;
          break;
      case DELLA_VOLUME:
          self->controlVolume = (float*)data;
          break;
      case DELLA_FEEDBACK:
          self->controlFeedback = (float*)data;
          break;
      case DELLA_ACTIVE:
          self->controlActive = (float*)data;
          break;
  }
}

void Della::run(LV2_Handle instance, uint32_t n_samples)
{
  Della* self = (Della*) instance;
  
  /// audio inputs
  float* in  = self->audioInput;
  float* out = self->audioOutput;
  
  /// control inputs
  float active    = *self->controlActive;
  float delay     = *self->controlDelay;
  float feedback  = *self->controlFeedback;
  
  if ( active > 0.5 )
    self->delay->active( true  );
  else
    self->delay->active( false );
  
  self->delay->setValue( delay );
  
  self->delay->setFeedback( feedback );
  
  self->delay->process( n_samples, in, out );
}

void Della::cleanup(LV2_Handle instance)
{
  delete ((Della*)instance)->delay;
  
  delete ((Della*) instance);
}

const void* Della::extension_data(const char* uri)
{
    return NULL;
}

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

#include "ducka.hxx"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

class Ducka
{
  public:
    Ducka(int rate);
    ~Ducka(){}
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
    float* audioSidechain;
    float* audioOutputL;
    float* audioOutputR;
    
    /// control signals
    float* controlThreshold;
    float* controlReduction;
    float* controlReleaseTime;
    float* controlSidechainAmp;
    
    /// filter state
    float w, a, b, g1, g2;
    
    /// last peak history
    bool nowIsAPeak;
    long peakFrameCounter;
    
    /// nframes available for countdown
    long peakCountDuration;
    
    /// control output
    float currentTarget;
};


static const LV2_Descriptor descriptor =
{
  DUCKA_URI,
  Ducka::instantiate,
  Ducka::connect_port,
  Ducka::activate,
  Ducka::run,
  Ducka::deactivate,
  Ducka::cleanup,
  Ducka::extension_data
};


LV2_SYMBOL_EXPORT const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
  if (index == 0) return &descriptor;
  else return NULL;
}


LV2_Handle Ducka::instantiate(const LV2_Descriptor* descriptor,
                              double samplerate,
                              const char* bundle_path,
                              const LV2_Feature* const* features)
{
  return (LV2_Handle) new Ducka( samplerate );
}

Ducka::Ducka(int rate) :
  /// filter state init
  w(10.0f / (rate * 0.02)),
  a(0.07f),
  b(1.0f / (1.0f - a)),
  g1(0.0f),
  g2(0.0f),
  
  peakFrameCounter(0),
  peakCountDuration( rate / 4 ),
  
  currentTarget(0)
{
}


void Ducka::activate(LV2_Handle instance)
{
}

void Ducka::deactivate(LV2_Handle instance)
{
}

void Ducka::connect_port(LV2_Handle instance, uint32_t port, void *data)
{
  Ducka* self = (Ducka*) instance;
  
  switch (port)
  {
      case DUCKA_INPUT_L:
          self->audioInputL = (float*)data;
          break;
      case DUCKA_INPUT_R:
          self->audioInputR = (float*)data;
          break;
      case DUCKA_SIDECHAIN:
          self->audioSidechain = (float*)data;
          break;
      case DUCKA_OUTPUT_L:
          self->audioOutputL = (float*)data;
          break;
      case DUCKA_OUTPUT_R:
          self->audioOutputR = (float*)data;
          break;
      
      case DUCKA_THRESHOLD:
          self->controlThreshold = (float*)data;
          break;
      case DUCKA_REDUCTION:
          self->controlReduction = (float*)data;
          break;
      
      case DUCKA_RELEASE_TIME:
          self->controlReleaseTime = (float*)data;
          break;
      case DUCKA_SIDECHAIN_AMP:
          self->controlSidechainAmp = (float*)data;
          break;
  }
}

void Ducka::run(LV2_Handle instance, uint32_t n_samples)
{
  Ducka* self = (Ducka*) instance;
  
  /// audio inputs
  float* inL  = self->audioInputL;
  float* inR  = self->audioInputR;
  float* side = self->audioSidechain;
  float* outL = self->audioOutputL;
  float* outR = self->audioOutputR;
  
  /// control inputs
  float threshold   = *self->controlThreshold;
  float reduction   = *self->controlReduction;
  float releaseTime = *self->controlReleaseTime;
  
  
  /// analyse sidechain input for peak
  float sum = 0.f;
  for( unsigned int i = 0; i < n_samples; i++ )
  {
    if ( *side > 0.000001 )
      sum += *side++;
    else
      sum += -*side++;
  }
  
  self->currentTarget = 0.f;
  
  /// check for peak level (offset to avoid "always on" peak)
  if ( sum / n_samples > threshold + 0.05 )
  {
    self->peakFrameCounter = self->peakCountDuration * releaseTime;
    self->currentTarget = 1.f - reduction;
  }
  else if ( self->peakFrameCounter < 0 )
  {
    self->currentTarget = 1.f;
  }
  else
  {
    self->currentTarget = 1.f - reduction;
  }
  
  if ( self->currentTarget < 0.f )
      self->currentTarget = 0.f;
  
  self->peakFrameCounter -= n_samples;
  
  for( unsigned int i = 0; i < n_samples; i++ )
  {
    /// smoothing algo is a lowpass, to de-zip the fades
    /// x^^4 approximates linear volume increase for human ears
    self->g1 += self->w * ( pow ( self->currentTarget, 4.f ) - self->g1 - self->a * self->g2 - 1e-20f);
    self->g2 += self->w * (self->b * self->g1 - self->g2 + 1e-20f);
    float gain = self->g2;
    
    *outL++ = *inL++ * gain;
    *outR++ = *inR++ * gain;
  }
  
  /// update output value
  *self->controlSidechainAmp = self->currentTarget;
}

void Ducka::cleanup(LV2_Handle instance)
{
  delete ((Ducka*) instance);
}

const void* Ducka::extension_data(const char* uri)
{
    return NULL;
}

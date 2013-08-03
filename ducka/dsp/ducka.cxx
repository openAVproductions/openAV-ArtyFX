
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
    float* controlTarget;
    float* controlReduction;
    float* controlReleaseTime;
    
    /// filter state
    float w, a, b, g1, g2;
    
    /// sidechain peak state
    float sidechainPeak;
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
  
  sidechainPeak(0.f)
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
      case DUCKA_TARGET:
          self->controlTarget    = (float*)data;
          break;
      case DUCKA_REDUCTION:
          self->controlReduction = (float*)data;
          break;
      
      case DUCKA_RELEASE_TIME:
          self->controlReleaseTime = (float*)data;
          break;
  }
}

void Ducka::run(LV2_Handle instance, uint32_t n_samples)
{
  Ducka* self = (Ducka*) instance;
  
  float currentTarget = 0.f;
  
  /// audio inputs
  float* inL  = self->audioInputL;
  float* inR  = self->audioInputR;
  float* side = self->audioSidechain;
  float* outL = self->audioOutputL;
  float* outR = self->audioOutputR;
  
  /// control inputs
  float* threshold   = self->controlThreshold;
  float* target      = self->controlTarget;
  float* reduction   = self->controlReduction;
  float* releaseTime = self->controlReleaseTime;
  
  for( int i = 0; i < n_samples; i++ )
  {
    /// analyse sidechain input here
    
    self->sidechainPeak = 0.f;
    
    
    /// smoothing algo is a lowpass, to de-zip the fades
    /// x^^4 approximates linear volume increase for human ears
    self->g1 += self->w * ( pow ( currentTarget, 4.f ) - self->g1 - self->a * self->g2 - 1e-20f);
    self->g2 += self->w * (self->b * self->g1 - self->g2 + 1e-20f);
    float gain = self->g2;
    
    *outL++ = *inL++ * gain;
    *outR++ = *inR++ * gain;
  }
  
}

void Ducka::cleanup(LV2_Handle instance)
{
  delete ((Ducka*) instance);
}

const void* Ducka::extension_data(const char* uri)
{
    return NULL;
}

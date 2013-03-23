/*
  Cutoff LV2 filter
  
  Copyright 2012 Harry van Haaren <harryhaaren@gmail.com>
  
  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THIS SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "cutoff.h"
#include "biquad.h"

typedef struct {
  // nessisary to generate a sin wave of the correct frequency
  double sample_rate;
  
  biquad* filter;
  
  // used internally to keep track of where we are in the wave
  float phase;
  
  float* type;
  float* freq;
  float* input;
  float* output;
} Cutoff;

static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
  Cutoff* self = (Cutoff*)malloc(sizeof(Cutoff));
  
  self->filter = malloc(sizeof(biquad));
  
  BiQuad_init( self->filter );
  
  // store the sample rate in "self" so we can retrieve it in run()
  self->sample_rate = rate;
  
  // initialize the phase so we start at the beginning of the wave
  self->phase = 0.f;
  
  return (LV2_Handle)self;
}

static void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void*      data)
{
  Cutoff* self = (Cutoff*)instance;

  switch ((PortIndex)port) {
  case CUTOFF_TYPE:
    self->type = (float*)data;
    break;
  case CUTOFF_INPUT:
    self->input = (float*)data;
    break;
  case CUTOFF_FREQ:
    self->freq = (float*)data;
    break;
  case CUTOFF_OUTPUT:
    self->output = (float*)data;
    break;
  }
}

static void
activate(LV2_Handle instance)
{
}

static void
run(LV2_Handle instance, uint32_t n_samples)
{
  Cutoff* self = (Cutoff*)instance;
  
  const int          type   = *(self->type);
  const float        freq   = *(self->freq);
  const float*       input  = self->input;
  float* const       output = self->output;
  
  // convert linear "freq" [0,1] to frequency (hertz)
  float noteFreq = (float)(24.f + (freq * 105.f));
  
  const float frequency = pow(2,((noteFreq-69)/12.f)) * 440.f;
  
  //printf("type %d, f %f  noteFreq %f  freq %f\n", type,  freq, noteFreq, frequency );
  
  BiQuad_setup( self->filter,
                type,               // type
                0,                  // dB gain
                frequency,          // significant freq
                self->sample_rate,  // sampling rate
                0.5     );          // bandwidth / octave
  
  // synthesize the sinwave here
  for (uint32_t i = 0; i < n_samples; i++)
  {
    output[i] = BiQuad( input[i], self->filter );
  }
}

static void
deactivate(LV2_Handle instance)
{
}

static void
cleanup(LV2_Handle instance)
{
  free(instance);
}

const void*
extension_data(const char* uri)
{
  return NULL;
}

static const LV2_Descriptor descriptor = {
  CUTOFF_URI,
  instantiate,
  connect_port,
  activate,
  run,
  deactivate,
  cleanup,
  extension_data
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
  switch (index) {
  case 0:
    return &descriptor;
  default:
    return NULL;
  }
}

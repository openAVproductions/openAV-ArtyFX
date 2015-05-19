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

#include "lv2/lv2plug.in/ns/ext/atom/atom.h"
#include "lv2/lv2plug.in/ns/ext/atom/util.h"
#include "lv2/lv2plug.in/ns/ext/urid/urid.h"
#include "lv2/lv2plug.in/ns/ext/time/time.h"

LV2_Handle Ducka::instantiate(const LV2_Descriptor* descriptor,
                              double samplerate,
                              const char* bundle_path,
                              const LV2_Feature* const* features)
{
  LV2_URID_Map*   m  = 0;
  LV2_URID_Unmap* un = 0;
  
  // get URID_map and unmap
  for (int i = 0; features[i]; ++i) {
    if (!strcmp(features[i]->URI, LV2_URID__map)) {
      m = (LV2_URID_Map*)features[i]->data;
    }
    if (!strcmp(features[i]->URI, LV2_URID__unmap)) {
      un = (LV2_URID_Unmap*)features[i]->data;
    }
  }
  
  Ducka* ducka = 0;
  
  if ( m )
  {
    ducka = new Ducka( samplerate, m );
  }
  else
  {
    printf("Ducka: Warning, your host does NOT support LV2_URID_Map. Ducka is\
            therefore unable to auto-sync to you're hosts BPM.");
    ducka = new Ducka( samplerate );
  }
  
  if ( un )
  {
    ducka->setUnmap(un);
  }
  
  
  return (LV2_Handle) ducka;
}

Ducka::Ducka(int rate) :
  /// filter state init
  w(10.0f / (rate * 0.02)),
  a(0.07f),
  b(1.0f / (1.0f - a)),
  g1(0.0f),
  g2(0.0f),
  
  samplerate( rate ),
  peakFrameCounter(0),
  peakCountDuration( rate / 4 ),
  
  currentTarget(0)
{
}

Ducka::Ducka(int rate, LV2_URID_Map* map) :
  /// filter state init
  w(10.0f / (rate * 0.02)),
  a(0.07f),
  b(1.0f / (1.0f - a)),
  g1(0.0f),
  g2(0.0f),
  
  samplerate( rate ),
  peakFrameCounter(0),
  peakCountDuration( rate / 4 ),
  
  currentTarget(0)
{
  time_Position      = map->map(map->handle, LV2_TIME__Position);
  time_barBeat       = map->map(map->handle, LV2_TIME__barBeat);
  time_beatsPerMinute= map->map(map->handle, LV2_TIME__beatsPerMinute);
  time_speed         = map->map(map->handle, LV2_TIME__speed);
  
  atom_Blank         = map->map(map->handle, LV2_ATOM__Blank);
  atom_Float         = map->map(map->handle, LV2_ATOM__Float);
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
      
      case DUCKA_ATOM_IN:
          self->atom_port = (LV2_Atom_Sequence*)data;
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
  
  
  /// handle Atom messages
  LV2_ATOM_SEQUENCE_FOREACH(self->atom_port, ev)
  {
    //printf("atom, %i\n", ev->body.type );
    if ( ev->body.type == self->atom_Blank ||
         ev->body.type == self->time_Position )
    {
      const LV2_Atom_Object* obj = (LV2_Atom_Object*)&ev->body;
      //printf("time_Position message\n" );
      LV2_Atom* bpm = 0;
      lv2_atom_object_get( (LV2_Atom_Object*)obj, self->time_beatsPerMinute, &bpm, NULL);
      
      if ( bpm )
      { 
        // Tempo changed, update BPM
        float bpmValue = ((LV2_Atom_Float*)bpm)->body;
        
        if ( bpmValue > 1 ) //protect against divide-by-zero segfault on host tempo 0 bpm
        {
          // set the new peakCountDuration, which is counted down from until fade up
          self->peakCountDuration = self->samplerate / ( bpmValue / 60.f);
        }
      }
      
    }
  }
  
  
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
  *self->controlSidechainAmp = 1-self->currentTarget;
}

void Ducka::cleanup(LV2_Handle instance)
{
  delete ((Ducka*) instance);
}

const void* Ducka::extension_data(const char* uri)
{
    return NULL;
}

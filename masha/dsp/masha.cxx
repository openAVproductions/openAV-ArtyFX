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

#include "lv2/lv2plug.in/ns/ext/atom/atom.h"
#include "lv2/lv2plug.in/ns/ext/atom/util.h"
#include "lv2/lv2plug.in/ns/ext/urid/urid.h"
#include "lv2/lv2plug.in/ns/ext/time/time.h"

class Masha
{
  public:
    Masha(int rate);
    Masha(int rate, LV2_URID_Map* map);
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
    
    /// Atom port
    LV2_URID time_Position;
    LV2_URID time_barBeat;
    LV2_URID time_beatsPerMinute;
    LV2_URID time_speed;
    
    LV2_URID atom_Blank;
    LV2_URID atom_Float;
    
    LV2_URID_Map* map;
    LV2_URID_Unmap* unmap;
    LV2_Atom_Sequence* atom_port;
    
    void setUnmap( LV2_URID_Unmap* um )
    {
      unmap = um;
    }
    
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
  
  Masha* masha = 0;
  
  if ( m )
  {
    masha = new Masha( samplerate, m );
  }
  else
  {
    printf("Masha: Warning, your host does NOT support LV2_URID_Map. Masha is\
            therefore unable to auto-sync to you're hosts BPM.");
    masha = new Masha( samplerate );
  }
  
  if ( un )
  {
    masha->setUnmap(un);
  }
  
  return (LV2_Handle) masha;
}

// allow instantiate without URID Map, but print warning
Masha::Masha(int rate)
{
  dspMasherL = new Masher( rate );
  dspMasherR = new Masher( rate );
  
  dspMasherL->bpm( 120 );
  dspMasherR->bpm( 120 );
}

Masha::Masha(int rate, LV2_URID_Map* map)
{
  dspMasherL = new Masher( rate );
  dspMasherR = new Masher( rate );
  
  dspMasherL->bpm( 120 );
  dspMasherR->bpm( 120 );
  
  time_Position      = map->map(map->handle, LV2_TIME__Position);
  time_barBeat       = map->map(map->handle, LV2_TIME__barBeat);
  time_beatsPerMinute= map->map(map->handle, LV2_TIME__beatsPerMinute);
  time_speed         = map->map(map->handle, LV2_TIME__speed);
  
  atom_Blank         = map->map(map->handle, LV2_ATOM__Blank);
  
  atom_Float         = map->map(map->handle, LV2_ATOM__Float);
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
      
      case MASHA_ATOM_IN:
          self->atom_port      = (LV2_Atom_Sequence*)data;
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
  
  /// handle Atom messages
  LV2_ATOM_SEQUENCE_FOREACH(self->atom_port, ev)
  {
    if ( ev->body.type == self->atom_Blank )
    {
      const LV2_Atom_Object* obj = (LV2_Atom_Object*)&ev->body;
      //printf("time_Position message\n" );
      LV2_Atom* bpm = 0;
      lv2_atom_object_get( (LV2_Atom_Object*)obj, self->time_beatsPerMinute, &bpm, NULL);
      
      if ( bpm ) //&& bpm->type == self->atom_Float) {
      {
        // Tempo changed, update BPM
        float bpmValue = ((LV2_Atom_Float*)bpm)->body;
        self->dspMasherL->bpm( bpmValue );
        self->dspMasherR->bpm( bpmValue );
        //printf("set bpm of %f\n", bpmValue );
      }
      
    }
    else
    {
      //printf("atom message: %s\n", self->unmap->unmap( self->unmap->handle, ev->body.type ) );
    }
  }
  
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

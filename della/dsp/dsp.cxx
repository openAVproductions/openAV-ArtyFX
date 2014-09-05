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

#include "lv2/lv2plug.in/ns/ext/atom/atom.h"
#include "lv2/lv2plug.in/ns/ext/atom/util.h"
#include "lv2/lv2plug.in/ns/ext/urid/urid.h"
#include "lv2/lv2plug.in/ns/ext/time/time.h"



LV2_Handle Della::instantiate(const LV2_Descriptor* descriptor,
                              double samplerate,
                              const char* bundle_path,
                              const LV2_Feature* const* features)
{
  Della* d = new Della( samplerate );
  d->map = 0;
  
  // get URID_map and unmap
  for (int i = 0; features[i]; i++)
  {
    if (!strcmp(features[i]->URI, LV2_URID__map))
    {
      d->map = (LV2_URID_Map*)features[i]->data;
    }
  }
  
  // we don't have the map extension: print warning
  if( !d->map )
  {
    printf("Della: Error: host doesn't provide Lv2 URID map, cannot sync BPM!\n");
    delete d;
    return 0;
  }
  
  // Get URID's for Lv2 TIME / ATOM extensions
  d->time_Position      = d->map->map(d->map->handle, LV2_TIME__Position);
  d->time_barBeat       = d->map->map(d->map->handle, LV2_TIME__barBeat);
  d->time_beatsPerMinute= d->map->map(d->map->handle, LV2_TIME__beatsPerMinute);
  d->time_speed         = d->map->map(d->map->handle, LV2_TIME__speed);
  
  d->atom_Blank         = d->map->map(d->map->handle, LV2_ATOM__Blank);
  d->atom_Float         = d->map->map(d->map->handle, LV2_ATOM__Float);
  
  return (LV2_Handle) d;
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
      
      case DELLA_ATOM_IN:
          self->atom_port      = (LV2_Atom_Sequence*)data;
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
  float volume    = *self->controlVolume;
  float feedback  = *self->controlFeedback;
  
  /// handle Atom messages
  LV2_ATOM_SEQUENCE_FOREACH(self->atom_port, ev)
  {
    if ( ev->body.type == self->atom_Blank )
    {
      const LV2_Atom_Object* obj = (LV2_Atom_Object*)&ev->body;
      //printf("time_Position message\n" );
      LV2_Atom* bpm = 0;
      lv2_atom_object_get(obj,
                          self->time_beatsPerMinute, &bpm,
                          NULL);
      
      if ( bpm ) //&& bpm->type == self->atom_Float) {
      {
        // Tempo changed, update BPM
        float bpmValue = ((LV2_Atom_Float*)bpm)->body;
        //self->dspMasherL->bpm( bpmValue );
        //printf("set bpm of %f\n", bpmValue );
        self->delay->setBPM( bpmValue );
      }
      
    }
    else
    {
      //printf("atom message: %s\n", self->unmap->unmap( self->unmap->handle, ev->body.type ) );
    }
  }
  
  if ( active > 0.5 )
    self->delay->active( true  );
  else
    self->delay->active( false );
  
  self->delay->setValue( delay );
  self->delay->setVolume( volume );
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

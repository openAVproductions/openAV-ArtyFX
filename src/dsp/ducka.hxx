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

#ifndef OPENAV_DUCKA
#define OPENAV_DUCKA

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define DUCKA_URI    "http://www.openavproductions.com/artyfx#ducka"
#define DUCKA_UI_URI "http://www.openavproductions.com/artyfx#ducka/gui"

typedef enum
{
  DUCKA_INPUT_L = 0,
  DUCKA_INPUT_R,
  
  DUCKA_SIDECHAIN,
  
  DUCKA_OUTPUT_L,
  DUCKA_OUTPUT_R,
  
  DUCKA_THRESHOLD,
  DUCKA_REDUCTION,
  DUCKA_RELEASE_TIME,
  
  DUCKA_SIDECHAIN_AMP,
  
  DUCKA_ATOM_IN,
} PortIndex;

#include "lv2/lv2plug.in/ns/ext/atom/atom.h"
#include "lv2/lv2plug.in/ns/ext/urid/urid.h"

class Ducka
{
  public:
    Ducka(int rate);
    Ducka(int rate, LV2_URID_Map* map);
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
    
    /// control signals
    float* controlThreshold;
    float* controlReduction;
    float* controlReleaseTime;
    float* controlSidechainAmp;
    
    /// filter state
    float w, a, b, g1, g2;
    
    /// last peak history
    long samplerate;
    bool nowIsAPeak;
    long peakFrameCounter;
    
    /// nframes available for countdown
    long peakCountDuration;
    
    /// control output
    float currentTarget;
};


#endif // OPENAV_DUCKA

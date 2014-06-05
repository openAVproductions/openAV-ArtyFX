/*
 * Author: Harry van Haaren 2014
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

#ifndef OPENAV_BITTA
#define OPENAV_BITTA

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define BITTA_URI    "http://www.openavproductions.com/artyfx#bitta"
#define BITTA_UI_URI "http://www.openavproductions.com/artyfx#bitta/gui"

typedef enum
{
  // audio
  BITTA_INPUT_L = 0,
  BITTA_OUTPUT_L,
  
  // controls
  BITTA_CRUSH,
  BITTA_ACTIVE,
  
  BITTA_ATOM_IN,
} BittaPortIndex;

#include "lv2/lv2plug.in/ns/ext/urid/urid.h"
#include "lv2/lv2plug.in/ns/ext/atom/atom.h"

class BitCrusher;

class Bitta
{
  public:
    Bitta(int rate);
    ~Bitta(){}
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
    float* audioOutputL;
    
    /// control signals
    float* controlCrush;
    float* controlActive;
    
    /// Atom port
    LV2_URID time_Position;
    LV2_URID time_barBeat;
    LV2_URID time_beatsPerMinute;
    LV2_URID time_speed;
    
    LV2_URID atom_Blank;
    LV2_URID atom_Float;
    
    LV2_URID_Map* map;
    LV2_Atom_Sequence* atom_port;
    
  private:
    /// runtime variables
    bool active;
    BitCrusher* bitcrusher;
};

#endif // OPENAV_BITTA

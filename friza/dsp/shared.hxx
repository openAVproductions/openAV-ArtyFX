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

#ifndef OPENAV_FRIZA
#define OPENAV_FRIZA

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define FRIZA_URI    "http://www.openavproductions.com/artyfx#friza"
#define FRIZA_UI_URI "http://www.openavproductions.com/artyfx#friza/gui"

typedef enum
{
  // Audio
  FRIZA_INPUT = 0,
  FRIZA_OUTPUT,
  
  // Control inputs
  FRIZA_DO_IT,
  FRIZA_LENGTH,
} FrizaPortIndex;

class SampleHoldShift;

class Friza
{
  public:
    Friza(int rate);
    ~Friza();
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
    
    /// control values
    float* doIt;
    float* length;
    //float* drive;
    //float* mix;
    
  private:
    SampleHoldShift* shs;
};

#endif // OPENAV_FRIZA

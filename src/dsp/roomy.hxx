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

#ifndef OPENAV_ROOMY
#define OPENAV_ROOMY

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define ROOMY_URI    "http://www.openavproductions.com/artyfx#roomy"
#define ROOMY_UI_URI "http://www.openavproductions.com/artyfx#roomy/gui"

typedef enum {
	ROOMY_INPUT_L = 0,
	ROOMY_INPUT_R,

	ROOMY_OUTPUT_L,
	ROOMY_OUTPUT_R,

	ROOMY_TIME,
	ROOMY_DAMPING,
	ROOMY_DRY_WET,
} RoomyPortIndex;


#include "dsp_reverb.hxx"

class Roomy
{
public:
	Roomy(int rate);
	~Roomy() {}
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
	float* controlDamping;
	float* controlDryWet;

private:
	Reverb dspReverb;
};

#endif // OPENAV_ROOMY

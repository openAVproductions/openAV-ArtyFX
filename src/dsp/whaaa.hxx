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

#ifndef OPENAV_WHAAA
#define OPENAV_WHAAA

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define WHAAA_URI    "http://www.openavproductions.com/artyfx#whaaa"
#define WHAAA_UI_URI "http://www.openavproductions.com/artyfx#whaaa/gui"

typedef enum {
	// Audio
	WHAAA_INPUT = 0,
	WHAAA_OUTPUT,

	// Control inputs
	WHAAA_FREQ,
	WHAAA_MIX,
} WhaaaPortIndex;

class Wah;

class Whaaa
{
public:
	Whaaa(int rate);
	~Whaaa();
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
	float* freq;
	float* mix;

private:
	Wah* wah;
};

#endif // OPENAV_WHAAA

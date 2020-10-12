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

#ifndef OPENAV_CAPTA
#define OPENAV_CAPTA

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define CAPTA_URI    "http://www.openavproductions.com/artyfx#capta"
#define CAPTA_UI_URI "http://www.openavproductions.com/artyfx#capta/gui"

typedef enum {
	// audio
	CAPTA_INPUT_1 = 0,
	CAPTA_INPUT_2,
	CAPTA_INPUT_3,
	CAPTA_INPUT_4,

	// controls
	CAPTA_RECORD,

} CaptaPortIndex;

#include "lv2/lv2plug.in/ns/ext/urid/urid.h"
#include "lv2/lv2plug.in/ns/ext/atom/atom.h"

#include "ringbuffer.h"
#include "zix/sem.h"
#include "zix/thread.h"

#ifndef _WIN32

class SndfileHandle;

class Capta
{
public:
	Capta(int rate);
	~Capta();
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
	float* audioInput1;
	float* audioInput2;
	float* audioInput3;
	float* audioInput4;

	/// control signals
	float* controlRecord;

private:
	const int sr;
	bool recording;

	/// ringbuffer for streaming all audio to disk
	jack_ringbuffer_t* ringbuffer;
	// used to copy the data to disk
	float* tmpAudioBuffer;

	/// disk thread
	SndfileHandle* sndfile;

	// signals start/stop recording
	ZixSem startRec;
	ZixSem stopRec;
	ZixSem overrunNotify;
	ZixSem quitDiskThread;

	// the disk thread
	ZixThread diskThread;

	void diskFunc();
	static void* staticDiskFunc(void* data)
	{
		((Capta*)data)->diskFunc();
		return 0;
	}
};

#endif

#endif // OPENAV_CAPTA

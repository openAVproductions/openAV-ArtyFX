/*
 * Author: Harry van Haaren 2015
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

#include "capta.hxx"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include <sstream>
#include <sndfile.hh>

LV2_Handle Capta::instantiate(const LV2_Descriptor* descriptor,
                              double samplerate,
                              const char* bundle_path,
                              const LV2_Feature* const* features)
{
	Capta* c = new Capta( samplerate );

	return (LV2_Handle)c;
}

Capta::Capta(int rate) :
	sr ( rate )
{
	recording = false;
	sndfile = 0;

	// 4 seconds ringbuffer - one second for each channel
	ringbuffer = jack_ringbuffer_create( sizeof(float)* rate * 4 );
	jack_ringbuffer_mlock( ringbuffer );
	jack_ringbuffer_reset( ringbuffer );

	tmpAudioBuffer = new float[rate * 4];

	zix_sem_init( &startRec       , 0 );
	zix_sem_init( &stopRec        , 0 );
	zix_sem_init( &quitDiskThread , 0 );
	zix_sem_init( &overrunNotify  , 0 );

	zix_thread_create( &diskThread, 4096, staticDiskFunc, this );
}

Capta::~Capta()
{
	// post sem to make disk thread quit
	zix_sem_post( &quitDiskThread );

	// wait for disk thread to quit
	zix_thread_join( diskThread, 0 );

	delete tmpAudioBuffer;
	jack_ringbuffer_free( ringbuffer );
}

void Capta::activate(LV2_Handle instance)
{
}

void Capta::deactivate(LV2_Handle instance)
{
}

void Capta::diskFunc()
{
	while( true ) {
		// check
		if( zix_sem_try_wait( &startRec ) ) {
			// start recording - make a new file, and start write thread etc
			time_t now = time(0);
			tm *ltm = localtime(&now);
			std::stringstream p;
			p << getenv("HOME") << "/" << 1900 +ltm->tm_year << "_" << 1 + ltm->tm_mon << "_" << ltm->tm_mday
			  << "_" << ltm->tm_hour << "_" << ltm->tm_min << "_" << ltm->tm_sec << ".wav";
			//printf( "Start record on %s \n", p.str().c_str() );

			sndfile = new SndfileHandle( p.str().c_str(), SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_FLOAT, 4, sr );
		}

		// handle data if available
		const size_t nsamps = jack_ringbuffer_read_space( ringbuffer );
		if( nsamps ) {
			jack_ringbuffer_read( ringbuffer, (char*)tmpAudioBuffer, nsamps );

			// if sndfile isn't allocated, we're not recording
			if( sndfile )
				sndfile->write( tmpAudioBuffer, nsamps/sizeof(float) );
		} else {
			usleep( 100000 );
		}

		// stop record?
		if( zix_sem_try_wait( &stopRec ) ) {
			sndfile->writeSync();
			delete sndfile;
			sndfile = 0;
		}

		if( zix_sem_try_wait( &overrunNotify ) ) {
			printf("Overruns in ringbuffer!\n" );
		}

		if( zix_sem_try_wait( &quitDiskThread ) ) {
			if( sndfile ) {
				//printf("disk thread recording on quit - cleaning up sndfile\n");
				delete sndfile;
			}
			//printf("disk thread quitting\n");
			return;
		}
	}
}

void Capta::connect_port(LV2_Handle instance, uint32_t port, void *data)
{
	Capta* self = (Capta*) instance;

	switch (port) {
	case CAPTA_INPUT_1:
		self->audioInput1  = (float*)data;
		break;
	case CAPTA_INPUT_2:
		self->audioInput2  = (float*)data;
		break;
	case CAPTA_INPUT_3:
		self->audioInput3  = (float*)data;
		break;
	case CAPTA_INPUT_4:
		self->audioInput4  = (float*)data;
		break;

	case CAPTA_RECORD:
		self->controlRecord= (float*)data;
		break;
	}
}

void Capta::run(LV2_Handle instance, uint32_t n_samples)
{
	Capta* self = (Capta*) instance;

	/// audio inputs
	float* in1 = self->audioInput1;
	float* in2 = self->audioInput2;
	float* in3 = self->audioInput3;
	float* in4 = self->audioInput4;

	/// control inputs
	float newRec = *self->controlRecord;

	if( !self->recording && newRec ) {
		zix_sem_post( &self->startRec );
	}
	if( self->recording && !newRec ) {
		zix_sem_post( &self->stopRec );
	}
	// save current rec state
	self->recording = newRec;

	if( self->recording ) {
		// in case n_samples == 0, don't notify of overrun
		int written = sizeof(float);

		// interleave write the samples - perhaps not optimized
		// but easy for writing in the disk-thread
		for ( unsigned int i = 0; i < n_samples; ++i ) {
			jack_ringbuffer_write( self->ringbuffer, (const char*)&in1[i], (size_t)sizeof(float) );
			jack_ringbuffer_write( self->ringbuffer, (const char*)&in2[i], (size_t)sizeof(float) );
			jack_ringbuffer_write( self->ringbuffer, (const char*)&in3[i], (size_t)sizeof(float) );
			written = jack_ringbuffer_write( self->ringbuffer, (const char*)&in4[i], (size_t)sizeof(float) );
		}

		if( written != sizeof(float) ) {
			zix_sem_post( &self->overrunNotify );
		}
	}

	return;
}

void Capta::cleanup(LV2_Handle instance)
{
	delete ((Capta*) instance);
}

const void* Capta::extension_data(const char* uri)
{
	return NULL;
}


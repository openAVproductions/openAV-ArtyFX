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

#include "satma.hxx"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dsp_distortion.hxx"

LV2_Handle Satma::instantiate(const LV2_Descriptor* descriptor,
                              double samplerate,
                              const char* bundle_path,
                              const LV2_Feature* const* features)
{
	return (LV2_Handle) new Satma( samplerate );
}

Satma::Satma(int rate)
{
	distortion = new Distortion( rate );
}

void Satma::activate(LV2_Handle instance)
{
}

void Satma::deactivate(LV2_Handle instance)
{
}

void Satma::connect_port(LV2_Handle instance, uint32_t port, void *data)
{
	Satma* self = (Satma*) instance;

	switch (port) {
	case SATMA_INPUT:
		self->audioInput    = (float*)data;
		break;
	case SATMA_OUTPUT:
		self->audioOutput   = (float*)data;
		break;

	case SATMA_DISTORTION:
		self->controlDistortion = (float*)data;
		break;
	case SATMA_TONE:
		self->controlTone       = (float*)data;
		break;
	case SATMA_ACTIVE:
		self->controlActive     = (float*)data;
		break;
	}
}

void Satma::run(LV2_Handle instance, uint32_t n_samples)
{
	Satma* self = (Satma*) instance;

	/// audio inputs
	float* in  = self->audioInput;
	float* out = self->audioOutput;

	/// control inputs
	float active     = *self->controlActive;
	float distortion = *self->controlDistortion;
	float tone       = *self->controlTone;

	if ( active > 0.5 )
		self->distortion->active( true  );
	else
		self->distortion->active( false );

	self->distortion->setValue( distortion );
	self->distortion->setEnvelope( tone );

	self->distortion->process( n_samples, in, out );
}

void Satma::cleanup(LV2_Handle instance)
{
	delete ((Satma*)instance)->distortion;

	delete ((Satma*) instance);
}

const void* Satma::extension_data(const char* uri)
{
	return NULL;
}

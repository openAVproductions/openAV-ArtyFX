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

#ifndef OPENAV_DSP_FILTERS_SVF_H
#define OPENAV_DSP_FILTERS_SVF_H

#include <math.h>
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

/** FiltersSVF
 *  This class contains a state-variable-filter implementation allowing faster
 *  modulation of parameters than the original Filters class. It also has added
 *  functionatlity of a resonance control.
 *
 *  A single filter produces high, band, notch and low outputs.
**/
class FiltersSVF
{
public:
	FiltersSVF(int sr) :
		samplerate( sr )
	{
		init();
		active = false;
		setValue( 1 );
		type = 0;
	}

	/** Sets the type of filter use
	 *  0: Low
	 *  1: High
	 *  2: Band
	 *  3: Notch
	 **/
	void setType(int t)
	{
		type = t;
		// turn off all
		volLowpass = volHighpass = volBandpass = volNotch = 0;
		if( t == 0 ) volLowpass  = 1;
		else if( t == 1 ) volHighpass = 1;
		else if( t == 2 ) volBandpass = 1;
		else if( t == 3 ) volNotch    = 1;
		else volLowpass = 1; // default, lowpass if bad selection made
	}

	int getType()
	{
		return type;
	}

	/// get functions for state
	float getFrequency()
	{
		return frequency;
	}
	float getResonance()
	{
		return resonance;
	}
	float getDrive    ()
	{
		return drive;
	}

	/// set frequency using float with range from 0 - 1
	void setValue( float v )
	{
		float midiNote = 24.f + v * 80.f;
		float tmp = 440.0*pow(2.0, (midiNote - 69.0)/12.0);

		setFrequency( tmp );
	}

	void setFrequency(float f)
	{
		frequency = f;
		if ( frequency < 40 ) frequency = 40;
		if ( frequency > samplerate / 2 - 200 ) frequency = samplerate / 2 - 200;

		// samplerate * 2 because it's double sampled
		impFreq = 2.0*sin(M_PI*min(0.25f, frequency/(samplerate*2.f)));
	}

	/// set resonance, range 0 - 1
	void setResonance( float r )
	{
		if ( r > 1.0 ) r = 1.0;
		if ( r < 0.0 ) r = 0.0;
		resonance = r;
		float tmpRes = r * 0.9;
		damping = min(2.0*(1.0 - pow(tmpRes, 0.25)), min(1.5, 2.0/impFreq - impFreq*0.5));
	}

	/// set drive / distrotion of filter, range 0 - 1
	void setDrive( float d )
	{
		if ( d > 1.0 ) d = 1.0;
		if ( d < 0.0 ) d = 0.0;
		//internal distortion 0 to 0.1
		drive  = d * 10; //100.f;
	}

	/// Processes audio when enabled, passes trough if not
	void setActive(bool a)
	{
		active = a;
	}

	int getNumInputs()
	{
		return 1;
	}
	int getNumOutputs()
	{
		return 1;
	}

	void process (long count, float* input, float* output)
	{
		for (int i=0; i < count; i++) {
			float in = input[i];
			float out = 0.f;

			notch = in - damping*band;
			low   = low + impFreq*band;
			high  = notch - low;
			band  = impFreq*high + band - drive*band*band*band;

			// FIXME: this needs volume mixing added
			out   = 0.5* ( (low*volLowpass) + (high*volHighpass) + (band*volBandpass) + (notch*volNotch) );

			notch = in - damping*band;
			low   = low + impFreq*band;
			high  = notch - low;
			band  = impFreq*high + band - drive*band*band*band;

			// FIXME: this needs volume mixing added
			out  += 0.5* ( (low*volLowpass) + (high*volHighpass) + (band*volBandpass) + (notch*volNotch) );

			output[i] = out;
		}
	}

private:
	/// sets the filters frequency
	float frequency;

	/// sets the resonance, range = 0-1. Internal range, 1-5 Q
	float resonance;

	/// sets the drive
	float drive;

	/// used to compare updated type with current type, for reset() or not
	int type;

	/// choose (or combine) filter characteristics using volume levels.
	float volLowpass;
	float volHighpass;
	float volBandpass;
	float volNotch;

	/// set the filter as active or not
	bool active;

	/// damping factor: not an input, co-efficient only
	float damping;

	// implementation variables
	// implementation frequency
	float impFreq;
	float notch;//  = notch output
	float low;//    = low pass output
	float high;//   = high pass output
	float band;//   = band pass output
	float peak;//   = peaking output = low - high

	const int samplerate;

	// sets up initial state:
	// -lowpass at 400 Hz
	// -small bit of resonance
	// -no drive
	void init()
	{
#warning VERIFY THESE DEFAULT VALUES
		band = 0.f;
		low = 0.f;

		// order is important: but for initial setup only!
		setFrequency( 400 );
		setResonance( 0.40 );
		setDrive( 0 );
		setType( 0 ); // lowpass
	}
};

#endif // OPENAV_DSP_FILTERS_H

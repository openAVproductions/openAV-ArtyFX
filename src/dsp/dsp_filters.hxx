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

/*
  The DSP code in this file was generated using FAUST.
  The FAUST code used is under the following licenses:

  filter.lib/author     Julius O. Smith (jos at ccrma.stanford.edu)
  filter.lib/copyright  Julius O. Smith III
  filter.lib/license    STK-4.3
  filter.lib/name       Faust Filter Library
  filter.lib/reference  https://ccrma.stanford.edu/~jos/filters/
  filter.lib/version    1.29

  math.lib/author       GRAME
  math.lib/copyright    GRAME
  math.lib/license      LGPL with exception
  math.lib/name         Math Library
  math.lib/version      1.0

  music.lib/author      GRAME
  music.lib/copyright   GRAME
  music.lib/license     LGPL with exception
  music.lib/name        Music Library
  music.lib/version     1.0
*/

#ifndef OPENAV_DSP_FILTERS_H
#define OPENAV_DSP_FILTERS_H

#include <math.h>
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

/** Filters
 * This class contains two filters, one lowpass, one highpass.
 * setValue() sets the cutoff:
 *  0.0 = lowpass down all the way
 *  0.5 = no change
 *  1.0 = highpass up all the way
**/
class Filters // : Effect
{
public:
	Filters(int sr)
	{
		init(sr);
		_active = false;
		value = 0.5;
	}

	void setValue(float v)
	{
		value = v;

		if ( _active ) {
			if ( value > 0.5 ) {
				float zeroOne = (value-0.5)*2.f;
				hipcutoff = 10 + pow(zeroOne,4) * 6000;
				lopcutoff = fSamplingFreq / 2.0;
			} else {
				float zeroOne = (value)*2.f;
				lopcutoff = 100 + pow(zeroOne,4) * ((fSamplingFreq/2.f)-100);
				hipcutoff = 10;
			}
		} else {
			hipcutoff = 10;
			lopcutoff = fSamplingFreq / 2.0;
		}
	}

	void active(bool a)
	{
		_active = a;

		// uncomment to re-calc frequency values on enable / disable
		// commented because the LV2 calls setValue() on *every* process()
		//setValue( value );
	}

	int getNumInputs()
	{
		return 2;
	}
	int getNumOutputs()
	{
		return 2;
	}

	void process (long count, float** inputs, float** outputs)
	{
		float* input0 = inputs[0];
		float* input1 = inputs[1];
		float* output0 = outputs[0];
		float* output1 = outputs[1];
		float fSlow0 = (fConst3 * float(hipcutoff));
		float fSlow1 = (fConst3 * float(lopcutoff));
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec4[0] = (fSlow0 + (fConst2 * fRec4[1]));
			float fTemp0 = tanf((fConst1 * fRec4[0]));
			float fTemp1 = (1.f / fTemp0);
			float fTemp2 = (1.f + ((fTemp1 - 1.84776f) / fTemp0));
			float fTemp3 = pow(fTemp0,2);
			float fTemp4 = (1.f / fTemp3);
			float fTemp5 = (1.f - fTemp4);
			float fTemp6 = (1.f + ((1.84776f + fTemp1) / fTemp0));
			fRec3[0] = (float(input0[i]) - (((fTemp2 * fRec3[2]) + (2.f * (fTemp5 * fRec3[1]))) / fTemp6));
			float fTemp7 = (0.f - fTemp4);
			float fTemp8 = (1.f + ((fTemp1 - 0.765367f) / fTemp0));
			float fTemp9 = (1.f + ((0.765367f + fTemp1) / fTemp0));
			fRec2[0] = (((((fRec3[0] / fTemp3) + (2.f * (fTemp7 * fRec3[1]))) + (fRec3[2] / fTemp3)) / fTemp6) - (((fTemp8 * fRec2[2]) + (2.f * (fTemp5 * fRec2[1]))) / fTemp9));
			fRec5[0] = (fSlow1 + (fConst2 * fRec5[1]));
			float fTemp10 = tanf((fConst1 * fRec5[0]));
			float fTemp11 = (1.f / fTemp10);
			float fTemp12 = (1.f + ((fTemp11 - 1.84776f) / fTemp10));
			float fTemp13 = (1.f - (1.f / pow(fTemp10,2)));
			float fTemp14 = (1.f + ((1.84776f + fTemp11) / fTemp10));
			fRec1[0] = (((((fRec2[0] / fTemp3) + (2.f * (fTemp7 * fRec2[1]))) + (fRec2[2] / fTemp3)) / fTemp9) - (((fTemp12 * fRec1[2]) + (2.f * (fTemp13 * fRec1[1]))) / fTemp14));
			float fTemp15 = (1.f + ((fTemp11 - 0.765367f) / fTemp10));
			float fTemp16 = (1.f + ((0.765367f + fTemp11) / fTemp10));
			fRec0[0] = (((fRec1[2] + (fRec1[0] + (2.f * fRec1[1]))) / fTemp14) - (((fTemp15 * fRec0[2]) + (2.f * (fTemp13 * fRec0[1]))) / fTemp16));
			output0[i] = float(((fRec0[2] + (fRec0[0] + (2.f * fRec0[1]))) / fTemp16));
			fRec9[0] = (float(input1[i]) - (((fTemp2 * fRec9[2]) + (2.f * (fTemp5 * fRec9[1]))) / fTemp6));
			fRec8[0] = (((((fRec9[0] / fTemp3) + (2.f * (fRec9[1] * fTemp7))) + (fRec9[2] / fTemp3)) / fTemp6) - (((fTemp8 * fRec8[2]) + (2.f * (fRec8[1] * fTemp5))) / fTemp9));
			fRec7[0] = (((((fRec8[0] / fTemp3) + (2.f * (fRec8[1] * fTemp7))) + (fRec8[2] / fTemp3)) / fTemp9) - (((fTemp12 * fRec7[2]) + (2.f * (fTemp13 * fRec7[1]))) / fTemp14));
			fRec6[0] = (((fRec7[2] + (fRec7[0] + (2.f * fRec7[1]))) / fTemp14) - (((fTemp15 * fRec6[2]) + (2.f * (fRec6[1] * fTemp13))) / fTemp16));
			output1[i] = float(((fRec6[2] + (fRec6[0] + (2.f * fRec6[1]))) / fTemp16));
			fRec4[1] = fRec4[0];
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fRec5[1] = fRec5[0];
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			fRec9[2] = fRec9[1];
			fRec9[1] = fRec9[0];
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
		}
	}

private:
	// 0.0 = lowpass down all the way
	// 0.5 = no change
	// 1.0 = highpass up all the way
	float value;

	bool _active;

	float fRec0[3];
	float fRec1[3];
	float fRec2[3];
	float fRec3[3];
	float fRec4[3];
	float fRec5[3];
	float fRec6[3];
	float fRec7[3];
	float fRec8[3];
	float fRec9[3];
	int fSamplingFreq;
	int iConst0;
	float fConst1;
	float fConst2;
	float fConst3;
	float hipcutoff;
	float lopcutoff;

	void init(int sr)
	{
		fSamplingFreq = sr;

		hipcutoff = 10.f;
		lopcutoff = 20000.f;

		iConst0 = min(192000, max(1, fSamplingFreq));
		fConst1 = (3.14159f / float(iConst0));
		fConst2 = expf((0.f - (20.f / float(iConst0))));
		fConst3 = (1.f - fConst2);

		for (int i = 0; (i < 2); i = (i + 1)) {
			fRec4[i] = 0.f;
		}
		for (int i = 0; (i < 3); i = (i + 1)) {
			fRec3[i] = 0.f;
		}
		for (int i = 0; (i < 3); i = (i + 1)) {
			fRec2[i] = 0.f;
		}
		for (int i = 0; (i < 2); i = (i + 1)) {
			fRec5[i] = 0.f;
		}
		for (int i = 0; (i < 3); i = (i + 1)) {
			fRec1[i] = 0.f;
		}
		for (int i = 0; (i < 3); i = (i + 1)) {
			fRec0[i] = 0.f;
		}
		for (int i = 0; (i < 3); i = (i + 1)) {
			fRec9[i] = 0.f;
		}
		for (int i = 0; (i < 3); i = (i + 1)) {
			fRec8[i] = 0.f;
		}
		for (int i = 0; (i < 3); i = (i + 1)) {
			fRec7[i] = 0.f;
		}
		for (int i = 0; (i < 3); i = (i + 1)) {
			fRec6[i] = 0.f;
		}
	}
};

#endif // OPENAV_DSP_FILTERS_H

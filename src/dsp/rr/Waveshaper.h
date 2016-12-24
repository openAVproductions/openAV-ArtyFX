/*
  ZynAddSubFX - a software synthesizer

  Waveshaper.h - Distortion Effect
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  Modified and evolved for rakarrack by Josep Andreu & Ryan Billing

  Modified to mono and LV2 by Harry van Haaren, 2013

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#ifndef WAVESHAPER_H
#define WAVESHAPER_H

#include "global.h"

class Waveshaper
{
public:
	Waveshaper (int sr);
	~Waveshaper ();

	//Waveshaping
	void waveshapesmps (int n, float * smps, int type,
	                    int drive, int eff);
	void cleanup();

	int period_coeff;

	double u_up;
	double u_down;

	float dthresh;  //dynamic threshold in compression waveshaper
	float dyno;
	float dynodecay;  //dynamically control symmetry

	float compg;  //used by compression distortion
	float cratio;  //used by compression for hardness
	float tmpgain;  // compression distortion temp variable
	float ncSAMPLE_RATE;
	float *temps;

	float R, P, Vgbias, Vsupp, Ip, Vmin, Vg, Vfactor, Vdyno;  //Valve1 Modeling variables.
	float mu, V2bias, Is, Vg2, vfact, ffact, Vlv2out, V2dyno; //Valve2 variables

	//class Resample *U_Resample;
	//class Resample *D_Resample;

	//int nframes;
	int samplerate;
};

#endif

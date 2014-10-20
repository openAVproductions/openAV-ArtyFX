/*
  ZynAddSubFX - a software synthesizer

  AnalogFilter.C - Several analog filters (lowpass, highpass...)
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  Modified for rakarrack by Ryan Billing and Josep Andreu
  
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

#include <math.h>
#include <stdio.h>
#include "config.h"
#include "AnalogFilter.h"


AnalogFilter::AnalogFilter (unsigned char Ftype, float Ffreq, float Fq,
                            unsigned char Fstages)
{
    // FIXME: SR
    iSAMPLE_RATE= 44100;
    ifSAMPLE_RATE=44100.f;


    stages = Fstages;
    for (int i = 0; i < 3; i++) {
        oldc[i] = 0.0;
        oldd[i] = 0.0;
        c[i] = 0.0;
        d[i] = 0.0;
    };
    
    type = Ftype;
    freq = Ffreq;
    q = Fq;
    gain = 1.0;
    if (stages >= MAX_FILTER_STAGES)
        stages = MAX_FILTER_STAGES;
    cleanup ();
    firsttime = 0;
    abovenq = 0;
    oldabovenq = 0;
    setfreq_and_q (Ffreq, Fq);
    firsttime = 1;
    d[0] = 0;			//this is not used
    outgain = 1.0;
    
    computefiltercoefs ();


};

AnalogFilter::~AnalogFilter ()
{
};

void
AnalogFilter::cleanup ()
{
    for (int i = 0; i < MAX_FILTER_STAGES + 1; i++) {
        x[i].c1 = 0.0;
        x[i].c2 = 0.0;
        y[i].c1 = 0.0;
        y[i].c2 = 0.0;
        oldx[i] = x[i];
        oldy[i] = y[i];
    };
    needsinterpolation = 0;
};

void
AnalogFilter::computefiltercoefs ()
{
    int zerocoefs = 0;		//this is used if the freq is too high
    float tmp;
    float omega, sn, cs, alpha, beta;

    //do not allow frequencies bigger than samplerate/2
    float freq = this->freq;
    if (freq > (iSAMPLE_RATE / 2 - 500.0)) {
        freq = ifSAMPLE_RATE * .5f - 500.0f;
        zerocoefs = 1;
    };
    if (freq < 0.1)
        freq = 0.1f;
    //do not allow bogus Q
    if (q < 0.0)
        q = 0.0;
    float tmpq, tmpgain;
    if (stages == 0) {
        tmpq = q;
        tmpgain = gain;
    } else {
        tmpq = (q > 1.0 ? powf (q, 1.0f /(float) (stages + 1)) : q);
        tmpgain = powf (gain, 1.0f / (float)(stages + 1));
    };

    //most of theese are implementations of
    //the "Cookbook formulae for audio EQ" by Robert Bristow-Johnson
    //The original location of the Cookbook is:
    //http://www.harmony-central.com/Computer/Programming/Audio-EQ-Cookbook.txt
    switch (type) {
    case 0:			//LPF 1 pole
        if (zerocoefs == 0)
            tmp = expf (-D_PI * freq / ifSAMPLE_RATE);
        else
            tmp = 0.0f;
        c[0] = 1.0f - tmp;
        c[1] = 0.0;
        c[2] = 0.0;
        d[1] = tmp;
        d[2] = 0.0;
        order = 1;
        break;
    case 1:			//HPF 1 pole
        if (zerocoefs == 0)
            tmp = expf (-D_PI * freq / ifSAMPLE_RATE);
        else
            tmp = 0.0f;
        c[0] = (1.0f + tmp) * .5f;
        c[1] = -(1.0f + tmp) * .5f;
        c[2] = 0.0f;
        d[1] = tmp;
        d[2] = 0.0f;
        order = 1;
        break;
    case 2:			//LPF 2 poles
        if (zerocoefs == 0) {
            omega = D_PI * freq / ifSAMPLE_RATE;
            sn = sinf (omega);
            cs = cosf (omega);
            alpha = sn / (2.0f * tmpq);
            tmp = 1 + alpha;
            c[0] = (1.0f - cs) * .5f / tmp;
            c[1] = (1.0f - cs) / tmp;
            c[2] = (1.0f - cs) * .5f / tmp;
            d[1] = -2.0f * cs / tmp * (-1.0f);
            d[2] = (1.0f - alpha) / tmp * (-1.0f);
        } else {
            c[0] = 1.0;
            c[1] = 0.0;
            c[2] = 0.0;
            d[1] = 0.0;
            d[2] = 0.0;
        };
        order = 2;
        break;
    case 3:			//HPF 2 poles
        if (zerocoefs == 0) {
            omega =  D_PI * freq / ifSAMPLE_RATE;
            sn = sinf (omega);
            cs = cosf (omega);
            alpha = sn / (2.0f * tmpq);
            tmp = 1.0f + alpha;
            c[0] = (1.0f + cs) / 2.0f / tmp;
            c[1] = -(1.0f + cs) / tmp;
            c[2] = (1.0f + cs) / 2.0f / tmp;
            d[1] = -2.0f * cs / tmp * (-1.0f);
            d[2] = (1.0f - alpha) / tmp * (-1.0f);
        } else {
            c[0] = 0.0;
            c[1] = 0.0;
            c[2] = 0.0;
            d[1] = 0.0;
            d[2] = 0.0;
        };
        order = 2;
        break;
    case 4:			//BPF 2 poles
        if (zerocoefs == 0) {
            omega = D_PI * freq / ifSAMPLE_RATE;
            sn = sinf (omega);
            cs = cosf (omega);
            alpha = sn / (2.0f * tmpq);
            tmp = 1.0f + alpha;
            c[0] = alpha / tmp * sqrtf (tmpq + 1.0f);
            c[1] = 0.0f;
            c[2] = -alpha / tmp * sqrtf (tmpq + 1.0f);
            d[1] = -2.0f * cs / tmp * (-1.0f);
            d[2] = (1.0f - alpha) / tmp * (-1.0f);
        } else {
            c[0] = 0.0;
            c[1] = 0.0;
            c[2] = 0.0;
            d[1] = 0.0;
            d[2] = 0.0;
        };
        order = 2;
        break;
    case 5:			//NOTCH 2 poles
        if (zerocoefs == 0) {
            omega = D_PI * freq / ifSAMPLE_RATE;
            sn = sinf (omega);
            cs = cosf (omega);
            alpha = sn / (2.0f * sqrtf (tmpq));
            tmp = 1.0f + alpha;
            c[0] = 1.0f / tmp;
            c[1] = -2.0f * cs / tmp;
            c[2] = 1.0f / tmp;
            d[1] = -2.0f * cs / tmp * (-1.0f);
            d[2] = (1.0f - alpha) / tmp * (-1.0f);
        } else {
            c[0] = 1.0;
            c[1] = 0.0;
            c[2] = 0.0;
            d[1] = 0.0;
            d[2] = 0.0;
        };
        order = 2;
        break;
    case 6:			//PEAK (2 poles)
        if (zerocoefs == 0) {
            omega = D_PI * freq / ifSAMPLE_RATE;
            sn = sinf (omega);
            cs = cosf (omega);
            tmpq *= 3.0f;
            alpha = sn / (2.0f * tmpq);
            tmp = 1.0f + alpha / tmpgain;
            c[0] = (1.0f + alpha * tmpgain) / tmp;
            c[1] = (-2.0f * cs) / tmp;
            c[2] = (1.0f - alpha * tmpgain) / tmp;
            d[1] = -2.0f * cs / tmp * (-1.0f);
            d[2] = (1.0f - alpha / tmpgain) / tmp * (-1.0f);
        } else {
            c[0] = 1.0;
            c[1] = 0.0;
            c[2] = 0.0;
            d[1] = 0.0;
            d[2] = 0.0;
        };
        order = 2;
        break;
    case 7:			//Low Shelf - 2 poles
        if (zerocoefs == 0) {
            omega = D_PI * freq / ifSAMPLE_RATE;
            sn = sinf (omega);
            cs = cosf (omega);
            tmpq = sqrtf (tmpq);
            alpha = sn / (2.0f * tmpq);
            beta = sqrtf (tmpgain) / tmpq;
            tmp = (tmpgain + 1.0f) + (tmpgain - 1.0f) * cs + beta * sn;

            c[0] =
                tmpgain * ((tmpgain + 1.0f) - (tmpgain - 1.0f) * cs +
                           beta * sn) / tmp;
            c[1] =
                2.0f * tmpgain * ((tmpgain - 1.0f) - (tmpgain + 1.0f) * cs) / tmp;
            c[2] =
                tmpgain * ((tmpgain + 1.0f) - (tmpgain - 1.0f) * cs -
                           beta * sn) / tmp;
            d[1] = -2.0f * ((tmpgain - 1.0f) + (tmpgain + 1.0f) * cs) / tmp * (-1.0f);
            d[2] =
                ((tmpgain + 1.0f) + (tmpgain - 1.0f) * cs - beta * sn) / tmp * (-1.0f);
        } else {
            c[0] = tmpgain;
            c[1] = 0.0;
            c[2] = 0.0;
            d[1] = 0.0;
            d[2] = 0.0;
        };
        order = 2;
        break;
    case 8:			//High Shelf - 2 poles
        if (zerocoefs == 0) {
            omega = D_PI * freq / ifSAMPLE_RATE;
            sn = sinf (omega);
            cs = cosf (omega);
            tmpq = sqrtf (tmpq);
            alpha = sn / (2.0f * tmpq);
            beta = sqrtf (tmpgain) / tmpq;
            tmp = (tmpgain + 1.0f) - (tmpgain - 1.0f) * cs + beta * sn;

            c[0] =
                tmpgain * ((tmpgain + 1.0f) + (tmpgain - 1.0f) * cs +
                           beta * sn) / tmp;
            c[1] =
                -2.0f * tmpgain * ((tmpgain - 1.0f) + (tmpgain + 1.0f) * cs) / tmp;
            c[2] =
                tmpgain * ((tmpgain + 1.0f) + (tmpgain - 1.0f) * cs -
                           beta * sn) / tmp;
            d[1] = 2.0f * ((tmpgain - 1.0f) - (tmpgain + 1.0f) * cs) / tmp * (-1.0f);
            d[2] =
                ((tmpgain + 1.0f) - (tmpgain - 1.0f) * cs - beta * sn) / tmp * (-1.0f);
        } else {
            c[0] = 1.0;
            c[1] = 0.0;
            c[2] = 0.0;
            d[1] = 0.0;
            d[2] = 0.0;
        };
        order = 2;
        break;
    default:			//wrong type
        type = 0;
        computefiltercoefs ();
        break;
    };
};


void
AnalogFilter::setfreq (float frequency)
{
    if (frequency < 0.1)
        frequency = 0.1f;
    float rap = freq / frequency;
    if (rap < 1.0)
        rap = 1.0f / rap;

    oldabovenq = abovenq;
    abovenq = frequency > (iSAMPLE_RATE / 2 - 500.0);

    int nyquistthresh = (abovenq ^ oldabovenq);


    if ((rap > 3.0) || (nyquistthresh != 0)) {
        //if the frequency is changed fast, it needs interpolation (now, filter and coeficients backup)
        for (int i = 0; i < 3; i++) {
            oldc[i] = c[i];
            oldd[i] = d[i];
        };
        for (int i = 0; i < MAX_FILTER_STAGES + 1; i++) {
            oldx[i] = x[i];
            oldy[i] = y[i];
        };
        if (firsttime == 0)
            needsinterpolation = 1;
    };
    freq = frequency;
    computefiltercoefs ();
    firsttime = 0;

};


void
AnalogFilter::setSR(unsigned int value)
{
    iSAMPLE_RATE = value;
    ifSAMPLE_RATE=(float) iSAMPLE_RATE;
    computefiltercoefs ();
}


void
AnalogFilter::setfreq_and_q (float frequency, float q_)
{
    q = q_;
    setfreq (frequency);
};

void
AnalogFilter::setq (float q_)
{
    q = q_;
    computefiltercoefs ();
};

void
AnalogFilter::settype (int type_)
{
    type = type_;
    computefiltercoefs ();
};

void
AnalogFilter::setgain (float dBgain)
{
    gain = dB2rap(dBgain);
    computefiltercoefs ();
};

void
AnalogFilter::setstages (int stages_)
{
    if (stages_ >= MAX_FILTER_STAGES)
        stages_ = MAX_FILTER_STAGES - 1;
    stages = stages_;
    cleanup ();
    computefiltercoefs ();
};

void
AnalogFilter::singlefilterout (float * smp, fstage & x, fstage & y,
                               float * c, float * d)
{
  /*
  if ( isinf( smp[0] ))
  {
    printf("smp is inf on start!\n");
  }
  */
    int i = 0;
    float y0 = smp[0];
    if (order == 1) {
        //First order filter
        for (i = 0; i < nframes; i++)
        {
            y0 = smp[i] * c[0] + x.c1 * c[1] + y.c1 * d[1];
            y.c1 = y0 + DENORMAL_GUARD;
            x.c1 = smp[i];
            //output
            smp[i] = y0;
        };
    };
    if (order == 2)
    {
        //Second order filter
        for (i = 0; i < nframes; i++)
        {
          /*
            bool nanBefore = false;
            if ( isnan( smp[i] ) )
            {
              nanBefore = true;
            }
            
            if ( isnan( y.c1 ) )
            {
              printf("y.c1 = NAN!\n");
              assert(false);
            }
            if ( isnan( d[1] ) )
            {
              printf(" d.[1] = NAN!\n");
              assert(false);
            }
            if ( isinf( y.c1 ) )
            {
              printf(" y.c1 = inf!\n");
              assert(false);
            }
            */
            
            //printf("smp[i] %f  * c[0] %f\n", smp[i], c[0] );
            //printf("%f  *   %f\n", y.c1, d[1] );
            
            y0 = (smp[i] * c[0]) + (x.c1 * c[1]) + (x.c2 * c[2]) + (y.c1 * d[1]) + (y.c2 * d[2]);
            
            /*
            if ( isnan( y0 )  )
            {
              printf("y0 = NAN!\n");
              assert(false);
            }
            if ( isinf( y0)  )
            {
              printf("y0 = inf!\n");
              assert(false);
            }
            */
            
            y.c2 = y.c1;
            y.c1 = y0 + DENORMAL_GUARD;
            
            x.c2 = x.c1;
            x.c1 = smp[i];
            

            
            //output
            smp[i] = y0;
        };
    };
};

void
AnalogFilter::filterout (int frames, float * smp)
{
    // nframes is class wide frame process amount
    nframes = frames;
    
    /*
    float *ismp = NULL;	//used if it needs interpolation
    if (needsinterpolation != 0) {
        ismp = new float[nframes];
        for (i = 0; i < nframes; i++)
            ismp[i] = smp[i];
        for (i = 0; i < stages + 1; i++)
            singlefilterout (ismp, oldx[i], oldy[i], oldc, oldd);
    };
    */
    int i;
    for (i = 0; i < stages + 1; i++)
    {
      singlefilterout ( smp, x[i], y[i], c, d);
    }
    /*
    if (needsinterpolation != 0) {
        for (i = 0; i < nframes; i++) {
            float x = (float) i / nframes;
            smp[i] = ismp[i] * (1.0f - x) + smp[i] * x;
        };
        delete (ismp);
        needsinterpolation = 0;
    };
    */
};


float
AnalogFilter::filterout_s(float smp)
{
    int i;
    if (needsinterpolation != 0) {
        for (i = 0; i < stages + 1; i++)
            smp=singlefilterout_s(smp, oldx[i], oldy[i], oldc, oldd);
    }

    for (i = 0; i < stages + 1; i++)
        smp=singlefilterout_s(smp, x[i], y[i], c, d);

    return(smp);

};



float
AnalogFilter::singlefilterout_s (float smp, fstage & x, fstage & y,
                                 float * c, float * d)
{
    float y0;
    if (order == 1) {
        //First order filter
        y0 = smp* c[0] + x.c1 * c[1] + y.c1 * d[1];
        y.c1 = y0;
        x.c1 = smp + DENORMAL_GUARD;
        //output
        smp = y0;

    };
    if (order == 2) {
        //Second order filter
        y0 =
            (smp* c[0]) + (x.c1 * c[1]) + (x.c2 * c[2]) + (y.c1 * d[1]) +
            (y.c2 * d[2]);
        y.c2 = y.c1;
        y.c1 = y0 + DENORMAL_GUARD;
        x.c2 = x.c1;
        x.c1 = smp;
        //output
        smp = y0;
    };

    return(smp);

};






void  AnalogFilter::reversecoeffs()
{
    float tmpd1, tmpd2, tmpc0;
    tmpd1 = -1.0f * d[1];
    tmpd2 = -1.0f * d[2];

    tmpc0 = 10.0f * c[0];

    c[0] = tmpc0;
    d[1] = -1.0f * c[1]*tmpc0;
    d[2] = -1.0f * c[2]*tmpc0;
    c[1] = tmpd1*tmpc0;
    c[2] = tmpd2*tmpc0;


};

float AnalogFilter::H (float freq)
{
    float
    fr = freq / ifSAMPLE_RATE * D_PI;
    float
    x = c[0], y = 0.0;
    for (int n = 1; n < 3; n++) {
        x += cosf ((float)n * fr) * c[n];
        y -= sinf ((float)n * fr) * c[n];
    };
    float
    h = x * x + y * y;
    x = 1.0;
    y = 0.0;
    for (int n = 1; n < 3; n++) {
        x -= cosf ((float)n * fr) * d[n];
        y += sinf ((float)n * fr) * d[n];
    };
    h = h / (x * x + y * y);
    return (powf (h, (float)(stages + 1) / 2.0f));
};



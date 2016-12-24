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

/** DSP : Linear Brown
 * This is a low-pass filtering solution to gradually change a value. A feedback
 * control is provided, and allows tailoring reaction speed and overshoot.
 */

#ifndef OPENAV_DSP_LINEAR_BROWN_H
#define OPENAV_DSP_LINEAR_BROWN_H

// Inspired from this article
// http://www.camelsoftware.com/firetail/blog/uncategorized/
// scalar-low-pass-filters-a-comparison-of-algorithms/

class BrownLinearExpo
{
public:
	BrownLinearExpo(double f = 0.0003) :
		a( f ),
		estimate( 1 ),
		single_smoothed(0.1),
		double_smoothed(0.1)
	{
	}

	/// set the factor (or speed) of following - ~0.0 to 0.02
	/// higher values cause more overshoot, but quicker response
	void set_factor(float f)
	{
		a = f;
	}

	/// sets the value - note that calling this will result in a jump in output
	void set_current( float c )
	{
		single_smoothed = c;
	}

	/// calculate the next output based on this input
	void step(float measurement)
	{
		single_smoothed = a * measurement + (1 - a) * single_smoothed;
		double_smoothed = a * single_smoothed + (1 - a) * double_smoothed;
		double est_a = (2*single_smoothed - double_smoothed);
		double est_b = (a / (1-a) )*(single_smoothed - double_smoothed);
		estimate = est_a + est_b;
	}

	/// retuns the current output
	float get()
	{
		return estimate;
	}

private:
	float a;
	float estimate;
	float single_smoothed;
	float double_smoothed;
};

#endif // OPENAV_DSP_LINEAR_BROWN_H


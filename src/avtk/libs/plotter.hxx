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

/** Util : Plotter
 * The plotter takes a range of data, and outputs it as a .dat file, with one
 * value in a single column. It then system() calls gnuplot to create a .png of
 * the data, and saves it as <int>_plot.png, where <int> is inremented by one.
 *
 * Created to easily view buffers' contents, this is a purely convienience class
 * to aid writing DSP code; it doesn't nothing useful of its own.
 *
 * The following python one-liner can be used to plot the graph:
 * python -c import pylab ; pylab.plot( open("plot.dat").readlines()[:-1] ) ; pylab.show()
 *
**/

#ifndef OPENAV_UTIL_PLOTTER_H
#define OPENAV_UTIL_PLOTTER_H

#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>


class Plotter
{
public:
	Plotter() :
		plotNum( 0 )
	{
	}

	static void plot( std::string name, long samples, const float* data )
	{
		printf("Plotter::plot() writing %s to %s\n", name.c_str(), getenv("PWD") );

		std::stringstream s;
		s << name << ".dat";

		std::ofstream outFile;
		outFile.open ( s.str().c_str() );

		for(long i = 0; i < samples; i++ )
			outFile << data[i] << "\n";

		outFile.close();
	}

	void showLast()
	{
		const char* pycommand = "";
		system( pycommand );
	}

private:
	int plotNum;

};

#endif // OPENAV_UTIL_PLOTTER_H

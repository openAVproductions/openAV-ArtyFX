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

#ifndef OPENAV_KUIZA
#define OPENAV_KUIZA

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define KUIZA_URI    "http://www.openavproductions.com/artyfx#kuiza"
#define KUIZA_UI_URI "http://www.openavproductions.com/artyfx#kuiza/gui"

typedef enum
{
  // audio
  KUIZA_INPUT = 0,
  KUIZA_OUTPUT,
  
  // controls
  KUIZA_GAIN,
  
  KUIZA_GAIN_S1,
  KUIZA_GAIN_S2,
  KUIZA_GAIN_S3,
  KUIZA_GAIN_S4,
  
  KUIZA_ACTIVE,
} PortIndex;

#endif // OPENAV_KUIZA

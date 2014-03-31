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

#ifndef OPENAV_VIHDA
#define OPENAV_VIHDA

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define VIHDA_URI    "http://www.openavproductions.com/artyfx#vihda"
#define VIHDA_UI_URI "http://www.openavproductions.com/artyfx#vihda/gui"

typedef enum
{
  // audio
  VIHDA_INPUT_L = 0,
  VIHDA_INPUT_R,
  VIHDA_OUTPUT_L,
  VIHDA_OUTPUT_R,
  
  // controls
  VIHDA_WIDTH,
  VIHDA_INVERT,
  VIHDA_ACTIVE,
  
  VIHDA_ATOM_IN,
} PortIndex;

#endif // OPENAV_VIHDA

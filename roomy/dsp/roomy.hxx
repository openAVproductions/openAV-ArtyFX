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

#ifndef OPENAV_ROOMY
#define OPENAV_ROOMY

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define ROOMY_URI    "http://www.openavproductions.com/artyfx#roomy"
#define ROOMY_UI_URI "http://www.openavproductions.com/artyfx#roomy/gui"

typedef enum
{
  ROOMY_INPUT_L = 0,
  ROOMY_INPUT_R,
  
  ROOMY_OUTPUT_L,
  ROOMY_OUTPUT_R,
  
  ROOMY_TIME,
  ROOMY_DAMPING,
  ROOMY_DRY_WET,
} PortIndex;

#endif // OPENAV_ROOMY

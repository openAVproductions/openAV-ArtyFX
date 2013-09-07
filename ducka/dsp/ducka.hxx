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

#ifndef OPENAV_DUCKA
#define OPENAV_DUCKA

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define DUCKA_URI    "http://www.openavproductions.com/artyfx#ducka"
#define DUCKA_UI_URI "http://www.openavproductions.com/artyfx#ducka/gui"

typedef enum
{
  DUCKA_INPUT_L = 0,
  DUCKA_INPUT_R,
  
  DUCKA_SIDECHAIN,
  
  DUCKA_OUTPUT_L,
  DUCKA_OUTPUT_R,
  
  DUCKA_THRESHOLD,
  DUCKA_REDUCTION,
  DUCKA_RELEASE_TIME,
  
  DUCKA_SIDECHAIN_AMP,
} PortIndex;

#endif // OPENAV_DUCKA

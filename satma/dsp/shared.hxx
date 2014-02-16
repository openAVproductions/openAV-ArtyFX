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

#ifndef OPENAV_SATMA
#define OPENAV_SATMA

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define SATMA_URI    "http://www.openavproductions.com/artyfx#satma"
#define SATMA_UI_URI "http://www.openavproductions.com/artyfx#satma/gui"

typedef enum
{
  SATMA_INPUT = 0,
  
  SATMA_OUTPUT,
  
  SATMA_DISTORTION,
  SATMA_TONE,
  SATMA_ACTIVE,
} PortIndex;

#endif // OPENAV_SATMA

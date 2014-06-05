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

// this file compiles into artyfx.so, including all plugins.


static const LV2_Descriptor descriptor =


static const LV2_Descriptor descriptors[] =
{
  {
  BITTA_URI,
  Bitta::instantiate,
  Bitta::connect_port,
  Bitta::activate,
  Bitta::run,
  Bitta::deactivate,
  Bitta::cleanup,
  Bitta::extension_data
  },
  {
    DELLA_URI,
    Della::instantiate,
    Della::connect_port,
    Della::activate,
    Della::run,
    Della::deactivate,
    Della::cleanup,
    Della::extension_data
  }
}


LV2_SYMBOL_EXPORT const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
  if (index == 0) return &descriptor;
  else return NULL;
}


LV2_SYMBOL_EXPORT const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
  if (index == 0) return &descriptor;
  else return NULL;
}

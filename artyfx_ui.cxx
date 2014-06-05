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

//  bitta/gui/ui.cxx bitta/gui/widget.cxx
// test compile:
// g++ artyfx_ui.cxx  bitta/gui/ui.cxx bitta/gui/widget.cxx della/gui/ui.cxx della/gui/widget.cxx  ducka/gui/ducka_ui.cxx ducka/gui/ducka_widget.cxx `pkg-config --cflags --libs sndfile cairomm-1.0 ntk ntk_images` -fPIC -shared -Wl,-z,nodelete  -Wl,--no-undefined -o artyfx.lv2/artyfx_ui.so

/// lv2 core / ui includes
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

// this file compiles into artyfx_ui.so, including all plugins' UIs.
#include "bitta/gui/ui.hxx"
#include "bitta/dsp/shared.hxx"

#include "della/gui/ui.hxx"
#include "della/dsp/shared.hxx"

#include "ducka/gui/ui.hxx"
#include "ducka/dsp/shared.hxx"

static LV2UI_Descriptor descriptors[] =
{
  {
    BITTA_UI_URI,
    bitta_instantiate,
    bitta_cleanup, 
    bitta_port_event, 
    bitta_extension_data
  },
  {
    DELLA_UI_URI,
    della_instantiate,
    della_cleanup,
    della_port_event,
    della_extension_data
  },
  {
    DUCKA_UI_URI,
    ducka_instantiate,
    ducka_cleanup,
    ducka_port_event,
    ducka_extension_data
  }
};

const LV2UI_Descriptor * lv2ui_descriptor(uint32_t index)
{
  if (index >= sizeof(descriptors) / sizeof(descriptors[0]))
  {
      return NULL;
  }
  return descriptors + index;
}

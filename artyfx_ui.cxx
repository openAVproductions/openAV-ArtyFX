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
// g++ artyfx_ui.cxx  bitta/gui/ui.cxx bitta/gui/widget.cxx della/gui/ui.cxx della/gui/widget.cxx  ducka/gui/ducka_ui.cxx ducka/gui/ducka_widget.cxx filta/gui/ui.cxx filta/gui/widget.cxx kuiza/gui/ui.cxx kuiza/gui/widget.cxx masha/gui/masha_widget.cxx masha/gui/masha_ui.cxx panda/gui/ui.cxx panda/gui/widget.cxx roomy/gui/roomy_ui.cxx roomy/gui/roomy_widget.cxx satma/gui/ui.cxx satma/gui/widget.cxx vihda/gui/ui.cxx vihda/gui/widget.cxx `pkg-config --cflags --libs sndfile cairomm-1.0 ntk ntk_images` -fPIC -shared -Wl,-z,nodelete  -Wl,--no-undefined -o artyfx.lv2/artyfx_ui.so

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

#include "driva/gui/ui.hxx"
#include "driva/dsp/shared.hxx"

#include "filta/gui/ui.hxx"
#include "filta/dsp/shared.hxx"

#include "friza/gui/ui.hxx"
#include "friza/dsp/shared.hxx"

#include "kuiza/gui/ui.hxx"
#include "kuiza/dsp/shared.hxx"

#include "masha/gui/ui.hxx"
#include "masha/dsp/shared.hxx"

#include "panda/gui/ui.hxx"
#include "panda/dsp/shared.hxx"

#include "roomy/gui/ui.hxx"
#include "roomy/dsp/shared.hxx"

#include "satma/gui/ui.hxx"
#include "satma/dsp/shared.hxx"

#include "vihda/gui/ui.hxx"
#include "vihda/dsp/shared.hxx"

#include "whaaa/gui/ui.hxx"
#include "whaaa/dsp/shared.hxx"

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
  },
  {
    DRIVA_UI_URI,
    driva_instantiate,
    driva_cleanup,
    driva_port_event,
    driva_extension_data
  },
  {
    FILTA_UI_URI,
    filta_instantiate,
    filta_cleanup,
    filta_port_event,
    filta_extension_data
  },
  {
    FRIZA_UI_URI,
    friza_instantiate,
    friza_cleanup,
    friza_port_event,
    friza_extension_data
  },
  {
    KUIZA_UI_URI,
    kuiza_instantiate,
    kuiza_cleanup,
    kuiza_port_event,
    kuiza_extension_data
  },
  {
    MASHA_UI_URI,
    masha_instantiate,
    masha_cleanup,
    masha_port_event,
    masha_extension_data
  },
  {
    PANDA_UI_URI,
    panda_instantiate,
    panda_cleanup,
    panda_port_event,
    panda_extension_data
  },
  {
    ROOMY_UI_URI,
    roomy_instantiate,
    roomy_cleanup,
    roomy_port_event,
    roomy_extension_data
  },
  {
    SATMA_UI_URI,
    satma_instantiate,
    satma_cleanup,
    satma_port_event,
    satma_extension_data
  },
  {
    VIHDA_UI_URI,
    vihda_instantiate,
    vihda_cleanup,
    vihda_port_event,
    vihda_extension_data
  },
  {
    WHAAA_UI_URI,
    whaaa_instantiate,
    whaaa_cleanup,
    whaaa_port_event,
    whaaa_extension_data
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

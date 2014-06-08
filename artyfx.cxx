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

/// this file compiles into artyfx.so, including all plugins.

// test compile
// g++ artyfx.cxx bitta/dsp/dsp.cxx della/dsp/dsp.cxx ducka/dsp/dsp.cxx filta/dsp/dsp.cxx kuiza/dsp/dsp.cxx kuiza/dsp/eq/filters.cc kuiza/dsp/eq/filters_if.cc kuiza/dsp/eq/exp2ap.cc masha/dsp/masha.cxx panda/dsp/dsp.cxx roomy/dsp/dsp.cxx satma/dsp/dsp.cxx vihda/dsp/dsp.cxx  -fPIC -shared  -Wl,--no-undefined -o artyfx.lv2/artyfx.so

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

// include each plugin
#include "bitta/dsp/shared.hxx"
#include "della/dsp/shared.hxx"
#include "ducka/dsp/shared.hxx"
#include "filta/dsp/shared.hxx"
#include "kuiza/dsp/shared.hxx"
#include "masha/dsp/shared.hxx"
#include "panda/dsp/shared.hxx"
#include "roomy/dsp/shared.hxx"
#include "satma/dsp/shared.hxx"
#include "vihda/dsp/shared.hxx"

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
  },
  {
    DUCKA_URI,
    Ducka::instantiate,
    Ducka::connect_port,
    Ducka::activate,
    Ducka::run,
    Ducka::deactivate,
    Ducka::cleanup,
    Ducka::extension_data
  },
  {
    FILTA_URI,
    Filta::instantiate,
    Filta::connect_port,
    Filta::activate,
    Filta::run,
    Filta::deactivate,
    Filta::cleanup,
    Filta::extension_data
  },
  {
    KUIZA_URI,
    Kuiza::instantiate,
    Kuiza::connect_port,
    Kuiza::activate,
    Kuiza::run,
    Kuiza::deactivate,
    Kuiza::cleanup,
    Kuiza::extension_data
  },
  {
    MASHA_URI,
    Masha::instantiate,
    Masha::connect_port,
    Masha::activate,
    Masha::run,
    Masha::deactivate,
    Masha::cleanup,
    Masha::extension_data
  },
  {
    PANDA_URI,
    Panda::instantiate,
    Panda::connect_port,
    Panda::activate,
    Panda::run,
    Panda::deactivate,
    Panda::cleanup,
    Panda::extension_data
  },
  {
    ROOMY_URI,
    Roomy::instantiate,
    Roomy::connect_port,
    Roomy::activate,
    Roomy::run,
    Roomy::deactivate,
    Roomy::cleanup,
    Roomy::extension_data
  },
  {
    SATMA_URI,
    Satma::instantiate,
    Satma::connect_port,
    Satma::activate,
    Satma::run,
    Satma::deactivate,
    Satma::cleanup,
    Satma::extension_data
  },
  {
    VIHDA_URI,
    Vihda::instantiate,
    Vihda::connect_port,
    Vihda::activate,
    Vihda::run,
    Vihda::deactivate,
    Vihda::cleanup,
    Vihda::extension_data
  }
};


LV2_SYMBOL_EXPORT const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
  if (index >= sizeof(descriptors) / sizeof(descriptors[0]))
  {
      return NULL;
  }
  return descriptors + index;
}


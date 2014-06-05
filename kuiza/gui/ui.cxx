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

#include <string>
#include <iostream>

#include <FL/x.H>

// include the URI and global data of this plugin
#include "../dsp/shared.hxx"

// this is our custom widget include
#include "widget.h"

// core spec include
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

// GUI
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

using namespace std;

typedef struct {
  KuizaWidget* widget;
  
  float sidechainAmp;
  
  LV2UI_Write_Function write_function;
  LV2UI_Controller controller;
} GUI;

LV2UI_Handle kuiza_instantiate(const struct _LV2UI_Descriptor * descriptor,
                const char * plugin_uri,
                const char * bundle_path,
                LV2UI_Write_Function write_function,
                LV2UI_Controller controller,
                LV2UI_Widget * widget,
                const LV2_Feature * const * features)
{
  if (strcmp(plugin_uri, KUIZA_URI) != 0) {
      fprintf(stderr, "KUIZA_URI error: this GUI does not support plugin with URI %s\n", plugin_uri);
      return NULL;
  }
  
  GUI* self = (GUI*)malloc(sizeof(GUI));
  if (self == NULL) return NULL;
  
  self->controller     = controller;
  self->write_function = write_function;
  
  void* parentXwindow = 0;
  LV2UI_Resize* resize = NULL;
  
  for (int i = 0; features[i]; ++i) {
    if (!strcmp(features[i]->URI, LV2_UI__parent)) {
      parentXwindow = features[i]->data;
    } else if (!strcmp(features[i]->URI, LV2_UI__resize)) {
      resize = (LV2UI_Resize*)features[i]->data;
    }
  }
  
  // in case FLTK hasn't opened it yet
  fl_open_display();
  
  self->widget = new KuizaWidget();
  
  self->widget->window->border(0);
  
  
  // write functions into the widget
  self->widget->controller = controller;
  self->widget->write_function = write_function;
  
  // set host to change size of the window
  if (resize)
  {
    resize->ui_resize(resize->handle, self->widget->getWidth(), self->widget->getHeight());
  }
  else
  {
    cout << "RoomyUI: Warning, host doesn't support resize extension.\n\
    Please ask the developers of the host to support this extension. "<< endl;
  }
  
  fl_embed( self->widget->window, (Window)parentXwindow );
  
  
  return (LV2UI_Handle)self;
}



void kuiza_cleanup(LV2UI_Handle ui) {
  GUI *pluginGui = (GUI *) ui;
  delete pluginGui->widget;
  free( pluginGui);
}

void kuiza_port_event(LV2UI_Handle ui,
               uint32_t port_index,
               uint32_t buffer_size,
               uint32_t format,
               const void * buffer)
{
  GUI *self = (GUI *) ui;
  
  if ( format == 0 )
  {
    float value =  *(float *)buffer;
    switch ( port_index )
    {
      case KUIZA_GAIN: {
            self->widget->graph->setGain( 0, value );
            self->widget->masterGain->value( value );
          } break;
      
      case KUIZA_GAIN_S1: {
            self->widget->graph->setGain( 1, value );
            self->widget->s1gain->value( value );
          } break;
      case KUIZA_GAIN_S2: {
            self->widget->graph->setGain( 2, value );
            self->widget->s2gain->value( value );
          } break;
      case KUIZA_GAIN_S3: {
            self->widget->graph->setGain( 3, value );
            self->widget->s3gain->value( value );
          } break;
      case KUIZA_GAIN_S4: {
            self->widget->graph->setGain( 4, value );
            self->widget->s4gain->value( value );
          } break;
      
      case KUIZA_ACTIVE: {
            self->widget->graph->setActive( value );
          } break;
      
    }
  }
  
  return;
}


int kuiza_idle(LV2UI_Handle handle)
{
  GUI* self = (GUI*)handle;
  
  self->widget->idle();
  
  return 0;
}

static const LV2UI_Idle_Interface idle_iface = { kuiza_idle };

const void* kuiza_extension_data(const char* uri)
{
  if (!strcmp(uri, LV2_UI__idleInterface))
  {
    return &idle_iface;
  }
  return NULL;
}


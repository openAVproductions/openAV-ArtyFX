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
  WhaaaWidget* widget;
  
  float sidechainAmp;
  
  LV2UI_Write_Function write_function;
  LV2UI_Controller controller;
} GUI;

LV2UI_Handle whaaa_instantiate(const struct _LV2UI_Descriptor * descriptor,
                const char * plugin_uri,
                const char * bundle_path,
                LV2UI_Write_Function write_function,
                LV2UI_Controller controller,
                LV2UI_Widget * widget,
                const LV2_Feature * const * features)
{
  if (strcmp(plugin_uri, WHAAA_URI) != 0) {
      fprintf(stderr, "WHAAA_URI error: this GUI does not support plugin with URI %s\n", plugin_uri);
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
  
  self->widget = new WhaaaWidget();
  
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



void whaaa_cleanup(LV2UI_Handle ui) {
  GUI *pluginGui = (GUI *) ui;
  delete pluginGui->widget;
  free( pluginGui);
}

void whaaa_port_event(LV2UI_Handle ui,
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
      case WHAAA_FREQ:
          {
            //self->widget->graph->value( value );
            self->widget->freq->value( value );
          }
          break;
      case WHAAA_RANGE:
          {
            self->widget->range->value( value );
          }
          break;
      case WHAAA_MIX:
          {
            self->widget->mix->value( value );
          }
          break;
      case WHAAA_DRIVE:
          {
            self->widget->drive->value( value );
          }
          break;
      
    }
  }
  
  return;
}


int whaaa_idle(LV2UI_Handle handle)
{
  GUI* self = (GUI*)handle;
  
  self->widget->idle();
  
  return 0;
}

const LV2UI_Idle_Interface idle_iface = { whaaa_idle };

const void* whaaa_extension_data(const char* uri)
{
  if (!strcmp(uri, LV2_UI__idleInterface))
  {
    return &idle_iface;
  }
  return NULL;
}


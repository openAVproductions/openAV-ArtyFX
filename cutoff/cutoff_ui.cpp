

// please see this link for details about the GUI aspect of LV2 programming
// http://harryhaaren.blogspot.ie/2012/07/writing-lv2-guis-making-it-look-snazzy.html

#include <string>
#include <iostream>

// X window ID
#define FL_INTERNALS 1
#include <FL/x.H>

// include the URI and global data of this plugin
#include "cutoff.h"

// this is our custom widget include
#include "cutoff_widget.h"

// core spec include
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

// GUI
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

#include "avtk/avtk.h"

using namespace std;

typedef struct {
    CutoffUI* widget;
    
    LV2UI_Write_Function write_function;
    LV2UI_Controller controller;
} CutoffGUI;

static LV2UI_Handle instantiate(const struct _LV2UI_Descriptor * descriptor,
                const char * plugin_uri,
                const char * bundle_path,
                LV2UI_Write_Function write_function,
                LV2UI_Controller controller,
                LV2UI_Widget * widget,
                const LV2_Feature * const * features)
{
    if (strcmp(plugin_uri, CUTOFF_URI) != 0) {
        fprintf(stderr, "SORCER_URI error: this GUI does not support plugin with URI %s\n", plugin_uri);
        return NULL;
    }
    
    CutoffGUI* self = (CutoffGUI*)malloc(sizeof(CutoffGUI));
    if (self == NULL) return NULL;
    
    int width  = 160;
    int height = 220;
    
    self->controller     = controller;
    self->write_function = write_function;
    
    void* parentXwindow = 0;
    LV2UI_Resize* resize = NULL;
    
    for (int i = 0; features[i]; ++i) {
      //cout << "feature " << features[i]->URI << endl;
      if (!strcmp(features[i]->URI, LV2_UI__parent)) {
        parentXwindow = features[i]->data;
      } else if (!strcmp(features[i]->URI, LV2_UI__resize)) {
        resize = (LV2UI_Resize*)features[i]->data;
      }
    }
    
    // in case FLTK hasn't opened it yet
    fl_open_display();
    
    //cout << "Creating UI!" << endl;
    self->widget = new CutoffUI();
    
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
      cout << "Cutoff: Warning, host doesn't support resize extension.\n\
      Please ask the developers of the host to support this extension. "<< endl;
    }
    
    fl_embed( self->widget->window, (Window)parentXwindow );
    
    
    return (LV2UI_Handle)self;
}



static void cleanup(LV2UI_Handle ui) {
    //printf("cleanup()\n");
    CutoffGUI *pluginGui = (CutoffGUI *) ui;
    delete pluginGui->widget;
    free( pluginGui);
}

static void port_event(LV2UI_Handle ui,
               uint32_t port_index,
               uint32_t buffer_size,
               uint32_t format,
               const void * buffer)
{
    CutoffGUI *self = (CutoffGUI *) ui;
    
    /*
    Fl::lock();
    ui->filterLowpass->value( argv[0]->f );
    Fl::unlock();
    Fl::awake();
    */
    
    /*
    if ( format == 0 )
    {
      float value =  *(float *)buffer;
      switch ( port_index )
      {
        case REFRACTOR_CONTROL_RETRIGGER:
            cout << "Refractor: Retrigger control event, value = " << value << endl;
            //self->guiState->retrigger = value;
            //self->widget->redraw();
            break;
        case REFRACTOR_MASTER_VOLUME:
            cout << "Refractor: Master volume event, value = " << value << endl;
            self->guiState->masterVol = value;
            self->widget->redraw();
      }
    }
    else
    {
      LV2_ATOM_SEQUENCE_FOREACH( (LV2_Atom_Sequence*)buffer, ev)
      {
        //self->frame_offset = ev->time.frames;
        
        if (ev->body.type == self->guiState->uris.midiEvent)
        {
          cout << "Refractor GUI got MIDI event!" << endl;
          //uint8_t* const data = (uint8_t* const)(ev + 1);
        }
      }
    }
    */
    
    return;
}


static int
idle(LV2UI_Handle handle)
{
  CutoffGUI* self = (CutoffGUI*)handle;
  
  self->widget->idle();
  
  return 0;
}

static const LV2UI_Idle_Interface idle_iface = { idle };

static const void*
extension_data(const char* uri)
{
  if (!strcmp(uri, LV2_UI__idleInterface)) {
    return &idle_iface;
  }
  return NULL;
}

static LV2UI_Descriptor descriptors[] = {
    {CUTOFF_UI_URI, instantiate, cleanup, port_event, extension_data}
};

const LV2UI_Descriptor * lv2ui_descriptor(uint32_t index) {
    if (index >= sizeof(descriptors) / sizeof(descriptors[0])) {
        return NULL;
    }
    return descriptors + index;
}

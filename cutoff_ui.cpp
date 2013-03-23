

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

#include "avtk/avtk_filter_lowpass.h"
#include "avtk/avtk_dial.h"
#include <FL/Fl_Dial.H>
#include "avtk/avtk_image.h"
#include "avtk/avtk_background.h"

using namespace std;

typedef struct {
    CutoffUI* widget;
    Fl_Window* win;
    
    LV2UI_Write_Function write_function;
    LV2UI_Controller controller;
    
    AvtkDial *freq;
    AvtkFilterLowpass *graph;
} CutoffGUI;

static CutoffGUI* static_self;

void dialCallback(Fl_Dial* o, void* v) {
  cout << o->value() << endl;
}

void cb_freq_i(Fl_Dial* o, void* s) {
  float tmp = o->value() * 2000 + 200;
  cout << "Controller " << static_self->controller << "   write_function " << static_self->write_function << endl;
  static_self->write_function( static_self->controller, CUTOFF_FREQ, sizeof(float), 0, (const void*)&tmp);
  static_self->graph->value(o->value());
}

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
    
    static_self = self;
    self->controller     = controller;
    self->write_function = write_function;
    
    cout << "Controller " << controller << "   write_function " << write_function << endl;
    
    void* parentXwindow = 0;
    LV2UI_Resize* resize = NULL;
    
    for (int i = 0; features[i]; ++i) {
      //cout << "feature " << features[i]->URI << endl;
      if (!strcmp(features[i]->URI, LV2_UI__parent)) {
        parentXwindow = features[i]->data;
        cout << "got parent UI feature: X11 id = " << (Window)parentXwindow << endl;
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
    if (resize) {
      //cout << "resizing Jalv window to : " << self->widget->getWidth() << " " << self->widget->getHeight() << endl;
      resize->ui_resize(resize->handle, self->widget->getWidth(), self->widget->getHeight());
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
    
    cout << "Port event on index " << port_index << "  Format is " << format << endl;
    
      
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
  cout << "UI extension data!" << endl;
	if (!strcmp(uri, LV2_UI__idleInterface)) {
    cout << "giving host idle interface!" << endl;
		return &idle_iface;
	}
	return NULL;
}

static LV2UI_Descriptor descriptors[] = {
    {CUTOFF_UI_URI, instantiate, cleanup, port_event, extension_data}
};

const LV2UI_Descriptor * lv2ui_descriptor(uint32_t index) {
    printf("lv2ui_descriptor(%u) called\n", (unsigned int)index); 
    if (index >= sizeof(descriptors) / sizeof(descriptors[0])) {
        return NULL;
    }
    return descriptors + index;
}

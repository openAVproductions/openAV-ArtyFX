
#include "../avtk/avtk.hxx"

/// lv2 core / ui includes
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

#define AVTK_UI_URI "http://www.openavproductions.com/avtk#testUI"

#include "../test_ui.hxx"


static LV2UI_Handle avtk_instantiate(const struct _LV2UI_Descriptor * descriptor,
                                     const char * plugin_uri,
                                     const char * bundle_path,
                                     LV2UI_Write_Function write_function,
                                     LV2UI_Controller controller,
                                     LV2UI_Widget * widget,
                                     const LV2_Feature * const * features)
{
	printf("init()\n");

	if (strcmp(plugin_uri, "http://www.openavproductions.com/avtk") != 0) {
		fprintf(stderr, "AVTK_UI_URI error: this GUI does not support plugin with URI %s\n", plugin_uri);
		return NULL;
	}

	LV2UI_Resize* resize = NULL;
	PuglNativeWindow parentXwindow = 0;
	for (int i = 0; features[i]; ++i) {
		printf("Feature %s\n", features[i]->URI );
		if (!strcmp(features[i]->URI, LV2_UI__parent)) {
			parentXwindow = (PuglNativeWindow)features[i]->data;
			printf("\tParent X11 ID %i\n", parentXwindow );
		} else if (!strcmp(features[i]->URI, LV2_UI__resize)) {
			resize = (LV2UI_Resize*)features[i]->data;
		}

	}
	//write_function, controller,
	TestUI* t = new TestUI( parentXwindow );

	*widget = (void*)t->getNativeHandle();

	printf("init() - returning\n");

	if (resize) {
		resize->ui_resize(resize->handle, t->w(), t->h() );
	}

	return t;
}

static void avtk_cleanup(LV2UI_Handle ui)
{
	printf("cleanup()\n");
	delete (TestUI*)ui;
}

static void avtk_port_event(LV2UI_Handle handle,
                            uint32_t port_index,
                            uint32_t buffer_size,
                            uint32_t format,
                            const void * buffer)
{
	TestUI* ui = (TestUI*)handle;
	printf("port()\n");
	ui->redraw();
}

static int avtk_idle(LV2UI_Handle handle)
{
	//printf("idle()\n");
	TestUI* ui = (TestUI*)handle;
	ui->idle();
	return 0;
}

static const LV2UI_Idle_Interface idle_iface = { avtk_idle };

static const void*
avtk_extension_data(const char* uri)
{
	if (!strcmp(uri, LV2_UI__idleInterface)) {
		return &idle_iface;
	}
	return NULL;
}

static const LV2UI_Descriptor descriptor = {
	AVTK_UI_URI,
	avtk_instantiate,
	avtk_cleanup,
	avtk_port_event,
	avtk_extension_data
};

LV2_SYMBOL_EXPORT const LV2UI_Descriptor* lv2ui_descriptor(uint32_t index)
{
	switch (index) {
	case 0:
		return &descriptor;
	default:
		return NULL;
	}
}


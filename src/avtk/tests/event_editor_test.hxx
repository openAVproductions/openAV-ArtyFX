
#ifndef OPENAV_AVTK_EVENT_EDITOR_TEST_HXX
#define OPENAV_AVTK_EVENT_EDITOR_TEST_HXX

#include "../avtk/avtk.hxx"

namespace Avtk
{
class Widget;
};

class EventEditorUI : public Avtk::UI
{
public:
	/// Set a NativeWindow for embedding: ignore for standalone
	EventEditorUI(PuglNativeWindow parent = 0);

	// ignore widget values, empty function body
	virtual void widgetValueCB( Avtk::Widget* widget) {}

	Avtk::Scroll* scroll;
	Avtk::EventEditor* editor;
};


#endif // OPENAV_AVTK_EVENT_EDITOR_TEST_HXX

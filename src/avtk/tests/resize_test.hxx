
#ifndef OPENAV_AVTK_RESIZE_TEST_HXX
#define OPENAV_AVTK_RESIZE_TEST_HXX

#include "../avtk/avtk.hxx"

namespace Avtk
{
class Widget;
};

class ResizeUI : public Avtk::UI
{
public:
	/// Set a NativeWindow for embedding: ignore for standalone
	ResizeUI(PuglNativeWindow parent = 0);

	// ignore widget values, empty function body
	virtual void widgetValueCB( Avtk::Widget* widget) {}

	Avtk::Dial* dial;
};


#endif // OPENAV_AVTK_RESIZE_TEST_HXX

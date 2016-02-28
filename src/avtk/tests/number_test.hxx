
#ifndef OPENAV_AVTK_NUMBER_TEST_HXX
#define OPENAV_AVTK_NUMBER_TEST_HXX

#include "../avtk/avtk.hxx"

namespace Avtk
{
class Widget;
};

class NumberUI : public Avtk::UI
{
public:
	/// Set a NativeWindow for embedding: ignore for standalone
	NumberUI(PuglNativeWindow parent = 0);

	// ignore widget values, empty function body
	virtual void widgetValueCB( Avtk::Widget* widget);

	Avtk::Number* number;
};


#endif // OPENAV_AVTK_NUMBER_TEST_HXX

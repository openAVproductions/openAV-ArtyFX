
#ifndef OPENAV_AVTK_LIST_TEST_HXX
#define OPENAV_AVTK_LIST_TEST_HXX

#include "../avtk/avtk.hxx"

namespace Avtk
{
class Widget;
};

class ListUI : public Avtk::UI
{
public:
	/// Set a NativeWindow for embedding: ignore for standalone
	ListUI(PuglNativeWindow parent = 0);

	// ignore widget values, empty function body
	virtual void widgetValueCB( Avtk::Widget* widget) {}

	Avtk::List* listH;
	Avtk::List* listV;
};


#endif // OPENAV_AVTK_LIST_TEST_HXX

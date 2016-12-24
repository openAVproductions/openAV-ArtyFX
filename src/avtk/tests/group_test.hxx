
#ifndef OPENAV_AVTK_GROUP_TEST_HXX
#define OPENAV_AVTK_GROUP_TEST_HXX

#include "../avtk/avtk.hxx"

namespace Avtk
{
class Widget;
};

class GroupUI : public Avtk::UI
{
public:
	/// Set a NativeWindow for embedding: ignore for standalone
	GroupUI(PuglNativeWindow parent = 0);
	~GroupUI() {};

	// ignore widget values, empty function body
	virtual void widgetValueCB( Avtk::Widget* widget) {}

	Avtk::Group* group1;
};


#endif // OPENAV_AVTK_GROUP_TEST_HXX

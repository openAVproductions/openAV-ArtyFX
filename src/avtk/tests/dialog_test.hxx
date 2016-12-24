
#ifndef OPENAV_AVTK_DIALOG_TEST_HXX
#define OPENAV_AVTK_DIALOG_TEST_HXX

#include "../avtk/avtk.hxx"

namespace Avtk
{
class Widget;
};

class DialogUI : public Avtk::UI
{
public:
	/// Set a NativeWindow for embedding: ignore for standalone
	DialogUI(PuglNativeWindow parent = 0);
	virtual ~DialogUI() {}

	virtual void widgetValueCB( Avtk::Widget* widget);

	Avtk::Button* show;
	Avtk::Dialog* dialog;
};


#endif // OPENAV_AVTK_DIALOG_TEST_HXX


#ifndef OPENAV_AVTK_KEY_TEST_HXX
#define OPENAV_AVTK_KEY_TEST_HXX

#include "../avtk/avtk.hxx"

namespace Avtk
{
class Widget;
};

class KeyUI : public Avtk::UI
{
public:
	KeyUI(PuglNativeWindow parent = 0);
	virtual ~KeyUI() {}
	int handle( const PuglEvent* e );
	void widgetValueCB( Avtk::Widget* w ) {}
};


#endif // OPENAV_AVTK_DIAL_KEY_HXX


#ifndef OPENAV_ARTYFX_MASHA_UI_HXX
#define OPENAV_ARTYFX_MASHA_UI_HXX

#include "avtk.hxx"

namespace Avtk
{
class Widget;
class Masher;
};

class MashaUI : public Avtk::UI
{
public:
	MashaUI(PuglNativeWindow parent = 0);
	virtual ~MashaUI() {}

	virtual void widgetValueCB( Avtk::Widget* widget);

	virtual void lv2PortEvent(  uint32_t index,
	                            uint32_t buffer_size,
	                            uint32_t format,
	                            const void* buffer );

	Avtk::Masher* graph;
	Avtk::Dial* dial1;
	Avtk::Dial* dial2;
	Avtk::Dial* dial3;
};


#endif // OPENAV_AVTK_DIAL_TEST_HXX

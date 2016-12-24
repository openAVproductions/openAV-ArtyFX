
#ifndef OPENAV_ARTYFX_WHAAA_UI_HXX
#define OPENAV_ARTYFX_WHAAA_UI_HXX

#include "avtk.hxx"

namespace Avtk
{
class Widget;
class Wah;
};

class WhaaaUI : public Avtk::UI
{
public:
	WhaaaUI(PuglNativeWindow parent = 0);
	virtual ~WhaaaUI() {}

	virtual void widgetValueCB( Avtk::Widget* widget);

	virtual void lv2PortEvent(  uint32_t index,
	                            uint32_t buffer_size,
	                            uint32_t format,
	                            const void* buffer );

	Avtk::Wah*  graph;
	Avtk::Dial* dial1;
	Avtk::Dial* dial2;
	Avtk::Dial* dial3;
};


#endif // OPENAV_ARTYFX_WHAAA_UI_HXX

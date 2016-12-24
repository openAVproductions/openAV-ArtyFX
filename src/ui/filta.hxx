
#ifndef OPENAV_ARTYFX_FILTA_UI_HXX
#define OPENAV_ARTYFX_FILTA_UI_HXX

#include "avtk.hxx"

namespace Avtk
{
class Widget;
class Filter;
};

class FiltaUI : public Avtk::UI
{
public:
	FiltaUI(PuglNativeWindow parent = 0);
	virtual ~FiltaUI() {}

	virtual void widgetValueCB( Avtk::Widget* widget);

	virtual void lv2PortEvent(  uint32_t index,
	                            uint32_t buffer_size,
	                            uint32_t format,
	                            const void* buffer );

	Avtk::Filter* graph;
	Avtk::Dial* dial1;
	Avtk::Dial* dial2;
};


#endif // OPENAV_ARTYFX_FILTA_UI_HXX

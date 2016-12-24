
#ifndef OPENAV_AVTK_BUTTON_HXX
#define OPENAV_AVTK_BUTTON_HXX

#include "widget.hxx"

namespace Avtk
{

class Button : public Widget
{
public:
	Button( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
	virtual ~Button() {}
	virtual void draw( cairo_t* cr );
};

};

#endif // OPENAV_AVTK_BUTTON_HXX

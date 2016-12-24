
#ifndef OPENAV_AVTK_SLIDER_HXX
#define OPENAV_AVTK_SLIDER_HXX

#include "widget.hxx"

namespace Avtk
{

class Slider : public Widget
{
public:
	Slider( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
	virtual ~Slider() {}

	virtual void draw( cairo_t* cr );
};

};

#endif // OPENAV_AVTK_SLIDER_HXX

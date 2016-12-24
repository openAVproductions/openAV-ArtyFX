
#ifndef OPENAV_AVTK_WIDENER_HXX
#define OPENAV_AVTK_WIDENER_HXX

#include "widget.hxx"

namespace Avtk
{

class Widener : public Widget
{
public:
	Widener( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
	virtual ~Widener() {}

	virtual void draw( cairo_t* cr );
	float width;
	float invert;
};

};

#endif // OPENAV_AVTK_WIDENER_HXX

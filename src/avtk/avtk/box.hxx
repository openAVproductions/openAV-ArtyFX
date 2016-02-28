
#ifndef OPENAV_AVTK_BOX_HXX
#define OPENAV_AVTK_BOX_HXX

#include "widget.hxx"

namespace Avtk
{

class Box : public Widget
{
public:
	Box( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
	virtual ~Box() {}
	virtual void draw( cairo_t* cr );
};

};

#endif // OPENAV_AVTK_BOX_HXX

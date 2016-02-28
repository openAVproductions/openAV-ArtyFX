
#ifndef OPENAV_AVTK_EQ_HXX
#define OPENAV_AVTK_EQ_HXX

#include "widget.hxx"

namespace Avtk
{

class Eq : public Widget
{
public:
	Eq( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
	virtual ~Eq() {}

	virtual void draw( cairo_t* cr );

	float gains[4];
};

};

#endif // OPENAV_AVTK_EQ_HXX

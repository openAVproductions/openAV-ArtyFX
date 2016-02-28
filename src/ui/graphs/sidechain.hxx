
#ifndef OPENAV_AVTK_SIDECHAIN_HXX
#define OPENAV_AVTK_SIDECHAIN_HXX

#include "widget.hxx"

namespace Avtk
{

class Sidechain : public Widget
{
public:
	Sidechain( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
	virtual ~Sidechain() {}

	virtual void draw( cairo_t* cr );

	// value() represents current reduction
	float threshold;
	float reduceAmount;
	float time;
};

};

#endif // OPENAV_AVTK_SIDECHAIN_HXX

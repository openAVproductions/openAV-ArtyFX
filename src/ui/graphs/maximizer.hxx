
#ifndef OPENAV_AVTK_MAXIMIZER_HXX
#define OPENAV_AVTK_MAXIMIZER_HXX

#include "widget.hxx"

namespace Avtk
{

class Maximizer : public Widget
{
public:
	Maximizer(Avtk::UI* ui, int x, int y, int w, int h, std::string label);
	virtual ~Maximizer() {}

	virtual void draw( cairo_t* cr );
	float tone;
};

};

#endif // OPENAV_AVTK_MAXIMIZER_HXX

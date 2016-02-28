
#ifndef OPENAV_AVTK_ENVELOPE_HXX
#define OPENAV_AVTK_ENVELOPE_HXX

#include "widget.hxx"

namespace Avtk
{

class Envelope : public Widget
{
public:
	Envelope( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
	virtual ~Envelope() {}

	virtual void draw( cairo_t* cr );

	void attack (float v)
	{
		a = v;
	}
	void decay  (float v)
	{
		d = v;
	}
	void sustain(float v)
	{
		s=1-v;
	}
	void release(float v)
	{
		r = v;
	}

private:
	float a;
	float d;
	float s;
	float r;
};

};

#endif // OPENAV_AVTK_ENVELOPE_HXX

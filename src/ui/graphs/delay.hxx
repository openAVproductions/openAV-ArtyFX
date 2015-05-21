
#ifndef OPENAV_AVTK_DELAY_HXX
#define OPENAV_AVTK_DELAY_HXX

#include "widget.hxx"

namespace Avtk
{

class Delay : public Widget
{
  public:
    Delay( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
    virtual ~Delay(){}
    
    virtual void draw( cairo_t* cr );

    float time;
    float feedback;
    float volume;
};

};

#endif // OPENAV_AVTK_DELAY_HXX

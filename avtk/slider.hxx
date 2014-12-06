
#ifndef OPENAV_AVTK2_SLIDER_HXX
#define OPENAV_AVTK2_SLIDER_HXX

#include "widget.hxx"

namespace Avtk
{

class Slider : public Widget
{
  public:
    Slider(int x, int y, int w, int h, std::string label);
    
    virtual void draw( cairo_t* cr );
};

};

#endif // OPENAV_AVTK2_SLIDER_HXX

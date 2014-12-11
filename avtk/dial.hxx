
#ifndef OPENAV_AVTK_DIAL_HXX
#define OPENAV_AVTK_DIAL_HXX

#include "widget.hxx"

namespace Avtk
{

class Dial : public Widget
{
  public:
    Dial( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
    
    virtual void draw( cairo_t* cr );
};

};

#endif // OPENAV_AVTK_DIAL_HXX

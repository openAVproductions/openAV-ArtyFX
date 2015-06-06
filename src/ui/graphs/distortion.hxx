
#ifndef OPENAV_AVTK_DISTORTION_HXX
#define OPENAV_AVTK_DISTORTION_HXX

#include "widget.hxx"

namespace Avtk
{

class Distortion : public Widget
{
  public:
    Distortion( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
    virtual ~Distortion(){}
    
    virtual void draw( cairo_t* cr );
};

};

#endif // OPENAV_AVTK_DISTORTION_HXX

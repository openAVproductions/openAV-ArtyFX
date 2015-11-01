
#ifndef OPENAV_AVTK_WAH_HXX
#define OPENAV_AVTK_WAH_HXX

#include "widget.hxx"

namespace Avtk
{

class Wah : public Widget
{
  public:
    Wah( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
    virtual ~Wah(){}
    
    virtual void draw( cairo_t* cr );
    // value == active
    float freq;
    float drive;
    float mix;
};

};

#endif // OPENAV_AVTK_WAH_HXX

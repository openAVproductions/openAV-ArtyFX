
#ifndef OPENAV_AVTK_REVERB_HXX
#define OPENAV_AVTK_REVERB_HXX

#include "widget.hxx"

namespace Avtk
{

class Reverb : public Widget
{
  public:
    Reverb( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
    virtual ~Reverb(){}
    
    virtual void draw( cairo_t* cr );

    float size;
    float damping;
    float dryWet;
};

};

#endif // OPENAV_AVTK_REVERB_HXX

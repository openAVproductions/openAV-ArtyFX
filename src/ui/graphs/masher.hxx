
#ifndef OPENAV_AVTK_MASHER_HXX
#define OPENAV_AVTK_MASHER_HXX

#include "widget.hxx"

namespace Avtk
{

class Masher : public Widget
{
  public:
    Masher( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
    virtual ~Masher(){}
    
    virtual void draw( cairo_t* cr );
    // value == active
    float time;
    float volume;
    float passthrough;
};

};

#endif // OPENAV_AVTK_MASHER_HXX

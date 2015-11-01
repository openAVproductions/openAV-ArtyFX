
#ifndef OPENAV_AVTK_COMPANDER_HXX
#define OPENAV_AVTK_COMPANDER_HXX

#include "widget.hxx"

namespace Avtk
{

class Compander : public Widget
{
  public:
    Compander( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
    virtual ~Compander(){}
    
    virtual void draw( cairo_t* cr );
    float release;
    float threshold;
};

};

#endif // OPENAV_AVTK_COMPANDER_HXX

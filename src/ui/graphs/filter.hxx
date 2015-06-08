
#ifndef OPENAV_AVTK_FILTER_HXX
#define OPENAV_AVTK_FILTER_HXX

#include "widget.hxx"

namespace Avtk
{

class Filter : public Widget
{
  public:
    Filter( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
    virtual ~Filter(){}
    
    virtual void draw( cairo_t* cr );
};

};

#endif // OPENAV_AVTK_FILTER_HXX


#ifndef OPENAV_AVTK_TEXT_HXX
#define OPENAV_AVTK_TEXT_HXX

#include "widget.hxx"

namespace Avtk
{

class Text : public Widget
{
  public:
    Text( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
    virtual ~Text(){}
    virtual void draw( cairo_t* cr );
};

};

#endif // OPENAV_AVTK_TEXT_HXX

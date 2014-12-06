
#ifndef OPENAV_AVTK2_BUTTON_HXX
#define OPENAV_AVTK2_BUTTON_HXX

#include "widget.hxx"

namespace Avtk
{

class Button : public Widget
{
  public:
    Button(int x, int y, int w, int h, std::string label);
    
    virtual void draw( cairo_t* cr ) override;
};

};

#endif // OPENAV_AVTK2_BUTTON_HXX

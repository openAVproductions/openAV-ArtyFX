
#ifndef OPENAV_AVTK_NUMBER_HXX
#define OPENAV_AVTK_NUMBER_HXX

#include "widget.hxx"

namespace Avtk
{

class Number : public Widget
{
  public:
    Number( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
    
    void setRange( int base, int amount );
    
    virtual void draw( cairo_t* cr );
  
  protected:
    int base;
    int amount;
};

};

#endif // OPENAV_AVTK_NUMBER_HXX

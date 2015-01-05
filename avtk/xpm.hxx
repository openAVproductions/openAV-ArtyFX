
#ifndef OPENAV_AVTK_XPM_HXX
#define OPENAV_AVTK_XPM_HXX

#include "widget.hxx"

namespace Avtk
{

class XPM : public Widget
{
  public:
    XPM( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
    virtual ~XPM();
    
    /// loads a XPM type 3 image into the widget, setting width and 
    void load( const char* data );
    
    virtual void draw( cairo_t* cr );
  
  private:
    cairo_surface_t* imgSurf;
    void* cairoImgData;
    int stride;
};

};

#endif // OPENAV_AVTK_XPM_HXX

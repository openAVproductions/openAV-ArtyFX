
#ifndef OPENAV_AVTK_IMAGE_HXX
#define OPENAV_AVTK_IMAGE_HXX

#include "widget.hxx"

namespace Avtk
{

class Image : public Widget
{
  public:
    Image(int x, int y, int w, int h, std::string label);
    
    /// assumes image is same size as this widget, and 4 bytes per pixel: RGBA
    void load( const unsigned char* data );
    
    virtual void draw( cairo_t* cr );
  
  private:
    cairo_surface_t* imgSurf;
};

};

#endif // OPENAV_AVTK_IMAGE_HXX

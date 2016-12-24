
#ifndef OPENAV_AVTK_IMAGE_HXX
#define OPENAV_AVTK_IMAGE_HXX

#include "widget.hxx"

namespace Avtk
{

class Image : public Widget
{
public:
	Image( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
	virtual ~Image();

	/// assumes image is same size as this widget, and 4 bytes per pixel: RGBA
	/// GIMP can save to <filename>.c, then choose (yes) RGBA/RGB, and enter a
	/// "prefix_name". The call load( prefix_name.pixel_data ); and ensure the
	/// file itself is being compiled into the program (just once!)
	void load( const unsigned char* data );

	virtual void draw( cairo_t* cr );

private:
	cairo_surface_t* imgSurf;
	void* cairoImgData;
	int stride;
};

};

#endif // OPENAV_AVTK_IMAGE_HXX

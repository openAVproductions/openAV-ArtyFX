
#include "image.hxx"

#include <stdio.h>

using namespace Avtk;

Image::Image( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
	Widget( ui, x_, y_, w_, h_, label_ ),
	imgSurf(0),
	cairoImgData(0)
{
	stride = cairo_format_stride_for_width (CAIRO_FORMAT_ARGB32, w_);
	cairoImgData = malloc (stride * h_);
}

Image::~Image()
{
#ifdef AVTK_DEBUG
	printf("%s\n", __PRETTY_FUNCTION__ );
#endif
	cairo_surface_destroy( imgSurf );
	free( cairoImgData );
}

void Image::load( const unsigned char* data )
{
#ifdef AVTK_DEBUG
	printf("w = %i, stride = %i\n", w_, stride );
#endif // AVTK_DEBUG
	memcpy( cairoImgData, data, sizeof(unsigned char) * w_ * h_ * 4 );
	imgSurf = cairo_image_surface_create_for_data( (unsigned char*)cairoImgData, CAIRO_FORMAT_ARGB32, w_, h_, stride);
}

void Image::draw( cairo_t* cr )
{
	if( !imgSurf ) {
#ifdef AVTK_DEBUG
		printf("Image::draw(), this = %i, imgSurf == 0\n", this );
#endif // AVTK_DEBUG
		return;
	}

	cairo_save( cr );
	cairo_set_source_surface( cr, imgSurf, x_, y_ );
	cairo_paint( cr );
	cairo_restore( cr );
}


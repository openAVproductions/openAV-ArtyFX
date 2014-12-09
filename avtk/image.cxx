
#include "image.hxx"

#include <stdio.h>

using namespace Avtk;

Image::Image( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ ),
  imgSurf(0),
  cairoImgData(0)
{
  stride = cairo_format_stride_for_width (CAIRO_FORMAT_ARGB32, w);
  cairoImgData = malloc (stride * h);
}

Image::~Image()
{
  free( cairoImgData );
}

void Image::load( const unsigned char* data )
{
  printf("w = %i, stride = %i\n", w, stride );
  memcpy( cairoImgData, data, sizeof(unsigned char)*w*h*4 );
  imgSurf = cairo_image_surface_create_for_data( (unsigned char*)cairoImgData, CAIRO_FORMAT_ARGB32, w, h, stride);
}

void Image::draw( cairo_t* cr )
{
  if( !imgSurf )
  {
    printf("Image::draw(), this = %i, imgSurf == 0\n", this );
    return;
  }
  
  cairo_save( cr );
  cairo_set_source_surface( cr, imgSurf, 0, 0 );
  cairo_paint( cr );
  cairo_restore( cr );
}


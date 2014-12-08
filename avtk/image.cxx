
#include "image.hxx"

#include <stdio.h>

using namespace Avtk;

Image::Image(int x_, int y_, int w_, int h_, std::string label_) :
  Widget( x_, y_, w_, h_, label_ ),
  imgSurf(0)
{
}

void Image::load( const unsigned char* data )
{
  imgSurf = cairo_image_surface_create_from_png ("header.png");
  
  /*
  int stride;
  
  stride = cairo_format_stride_for_width (CAIRO_FORMAT_RGB24, w);
  //newData = malloc (stride * height);
  //memcpy( newData, data, sizeof(unsigned char)*w*h*3 );
  imgSurf = cairo_image_surface_create_for_data( (unsigned char*)data, CAIRO_FORMAT_RGB24, w, h, stride);
  */
}

void Image::draw( cairo_t* cr )
{
  
  /*
  // from http://kapo-cpp.blogspot.ie/2007/11/drawing-images-to-cairo-context.html
  context->save();
  context->set_source (image_surface_ptr_, 0.0, 0.0);
  context->rectangle (0.0, 0.0, image_width, image_height);
  context->clip();
  context->paint();
  context->restore();
  */
  
  if( !imgSurf )
  {
    printf("Image::draw(), !imgSurf, loading now\n");
    imgSurf = cairo_image_surface_create_from_png ("header.png");
    printf(" imgSurf now %i\n", imgSurf );
  }
  
  /*
  if( !cairo_surface_write_to_png( imgSurf, "header.png" ) )
  {
    printf("Image::write_to_png() ERROR\n");
  }
  */
  
  cairo_set_source_surface( cr, imgSurf, 0, 0 );
  cairo_rectangle( cr, 0, 0, w, h );
  cairo_paint( cr );
}


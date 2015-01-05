
#include "xpm.hxx"

#include <stdio.h>
#include <stdint.h>

using namespace Avtk;

XPM::XPM( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ ),
  imgSurf(0),
  cairoImgData(0)
{
  stride = cairo_format_stride_for_width (CAIRO_FORMAT_ARGB32, w_);
  cairoImgData = malloc (stride * h_);
}

XPM::~XPM()
{
#ifdef AVTK_DEBUG
  printf("%s\n", __PRETTY_FUNCTION__ );
#endif
  cairo_surface_destroy( imgSurf );
  free( cairoImgData );
}

// inspired from Ardour / gtk_ardour2/utils.cc
void XPM::load( const char* constXpm )
{
  char* xpm = strdup( constXpm );
#ifdef AVTK_DEBUG
  printf("w = %i, stride = %i\n", w_, stride );
#endif // AVTK_DEBUG
  
  static long vals[256], val;
  uint32_t t, x, y, colors, cpp;
  unsigned char c;
  unsigned char *savergb, *rgb;
  char transparent;
  int wide;
  int high;

  // PARSE HEADER

  if ( sscanf( &xpm[0], "%u%u%u%u", &wide, &high, &colors, &cpp) != 4 )
  {
    printf( "bad XPM header %1", &xpm[0] );
    free( xpm );
    return;
  }
  
  savergb = rgb = (unsigned char*) malloc (high * wide * 4);
  
  // load XPM colormap for conversion
  if( strstr (&xpm[1], "None") )
  {
    sscanf ( &xpm[1], "%c", &transparent);
    t = 1;
  }
  else
  {
    transparent = 0;
    t = 0;
  }
  
  for (; t < colors; ++t)
  {
    sscanf( &xpm[t+1], "%c c #%lx", &c, &val);
    vals[c] = val;
  }
  
  // COLORMAP -> RGB CONVERSION
  //    Get low 3 bytes from vals[]
  //
  const char *p;
  for (y = high-1; y > 0; --y)
  {
    char alpha;
    
    for( p = &xpm[1+colors+(high-y-1)], x = 0; x < wide; x++, rgb += 4)
    {
      if (transparent && (*p++ == transparent))
      {
        alpha = 0;
        val = 0;
      }
      else
      {
        alpha = 255;
        val = vals[(int)*p];
      }
      
      *(rgb+3) = alpha;                  // 3: alpha
      *(rgb+2) = val & 0xff; val >>= 8;  // 2:B
      *(rgb+1) = val & 0xff; val >>= 8;  // 1:G
      *(rgb+0) = val & 0xff;             // 0:R
    }
  }
  
  //memcpy( cairoImgData, data, sizeof(unsigned char) * w_ * h_ * 4 );
  //imgSurf = cairo_image_surface_create_for_data( (unsigned char*)cairoImgData, CAIRO_FORMAT_ARGB32, w_, h_, stride);
  
  free( xpm );
}

void XPM::draw( cairo_t* cr )
{
  if( !imgSurf )
  {
#ifdef AVTK_DEBUG
    printf("XPM::draw(), this = %i, imgSurf == 0\n", this );
#endif // AVTK_DEBUG
    return;
  }
  
  cairo_save( cr );
  cairo_set_source_surface( cr, imgSurf, 0, 0 );
  cairo_paint( cr );
  cairo_restore( cr );
}


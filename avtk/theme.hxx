
#ifndef OPENAV_AVTK_THEME_HXX
#define OPENAV_AVTK_THEME_HXX

#include <cairo/cairo.h>

namespace Avtk
{

/// each color gets a use-case. To draw using said color
enum USE_CASE
{
  BG = 0,
  FG,
  
  SHADOW,
  OUTLINE,
  
  WARN,
  ERROR,
  
  USE_CASE_COUNT,
};

/// a Theme instance is a set color swatch that can be applied
class Theme
{
  public:
    Theme( std::string name = "OpenAV Default" ) :
      alpha_( 1 ),
      cornerRadius_( 1 )
    {}
    
    virtual int cornerRadius()
    {
      return cornerRadius_;
    }
    
    virtual void cornerRadius( int c )
    {
      cornerRadius_;
    }
    
    virtual void alpha( float a )
    {
      alpha_ = a;
    }
    
    virtual void fg( cairo_t* cr )
    {
      cairo_set_source_rgba(cr,  76/255., 80/255., 83/255., alpha_);
    }
    virtual void fgDark( cairo_t* cr )
    {
      cairo_set_source_rgba(cr,  35/255., 87/255., 136/255., alpha_);
    }
    virtual void bg( cairo_t* cr )
    {
      cairo_set_source_rgba(cr,  34/255.,  34/255.,  34/255., alpha_);
    }
    virtual void bgDark( cairo_t* cr )
    {
      cairo_set_source_rgba(cr,  24/255.,  24/255.,  24/255., alpha_);
    }
    virtual void highlight( cairo_t* cr )
    {
      cairo_set_source_rgba(cr,  0/255., 128/255., 255/255., alpha_);
    }
    
    virtual void transparent( bool t )
    {
      if( t )
        alpha_ = 0.4;
      else
        alpha_ = 1;
    }
    
    int cornerRadius_;
    float alpha_;
};

};

#endif // OPENAV_AVTK_THEME_HXX

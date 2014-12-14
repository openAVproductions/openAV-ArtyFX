
#ifndef OPENAV_AVTK_THEME_HXX
#define OPENAV_AVTK_THEME_HXX

#include <string>
#include <stdio.h>
#include <cairo/cairo.h>

namespace Avtk
{

/// forward declaration of the class
class UI;

/// each color gets a use-case. To draw using said color
enum USE_CASE
{
  BG = 0,
  BG_DARK,
  FG,
  FG_DARK,
  
  HIGHLIGHT,
  
  SHADOW,
  OUTLINE,
  
  LINE_WIDTH_THIN,
  LINE_WIDTH_WIDE,
  
  CORNER_RADIUS,
  
  USE_CASE_COUNT,
};

/// a Theme instance is a set color swatch that can be applied
class Theme
{
  public:
    Theme( Avtk::UI* ui_, std::string name = "OpenAV Default" ) :
      cornerRadius_( 1 ),
      lineWidthThin_( 0.9 ),
      lineWidthNorm_( 1.1 ),
      lineWidthWide_( 2.1 ),
      
      ui( ui_ )
    {}
    
    float lineWidthThin(){ return lineWidthThin_; }
    float lineWidthNorm(){ return lineWidthNorm_; }
    float lineWidthWide(){ return lineWidthWide_; }
    
    float color( cairo_t* cr, USE_CASE uc, float alpha = 1.0 )
    {
      float handled = setColor( cr, uc, alpha );
      
      if ( !handled )
      {
        return useDefaultColor( cr, uc, alpha );
      }
      
      return handled;
    }
    
    // so themes can override this function to set custom colors. The derived
    // class must return non-zero if it handled the colour
    virtual float setColor( cairo_t* cr, USE_CASE uc, float alpha )
    {
      return 0;
    }
    
    /// default theme, values returned using float ret value 
    float useDefaultColor( cairo_t* cr, USE_CASE uc, float alpha_ );
    
    void cornerRadius( int c );
    
    int cornerRadius_;
    float lineWidthThin_;
    float lineWidthNorm_;
    float lineWidthWide_;
    
    Avtk::UI* ui;
};

};

#endif // OPENAV_AVTK_THEME_HXX


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
  
  LINE_WIDTH_THIN,
  LINE_WIDTH_WIDE,
  
  CORNER_RADIUS,
  
  USE_CASE_COUNT,
};

struct Color
{
  /// r, g, b
  float c[3];
};

/// a Theme instance is a set color swatch that can be applied
class Theme
{
  public:
    Theme( Avtk::UI* ui_, std::string path );
    virtual ~Theme(){}
    
    float lineWidthThin(){ return lineWidthThin_; }
    float lineWidthNorm(){ return lineWidthNorm_; }
    float lineWidthWide(){ return lineWidthWide_; }
    
    float color( cairo_t* cr, USE_CASE uc, float alpha = 1.0 );
    
    void cornerRadius( int c );
    
    int cornerRadius_;
    float lineWidthThin_;
    float lineWidthNorm_;
    float lineWidthWide_;
    
    Avtk::UI* ui;
  
  private:
    static int privateID;
    int ID;
    
    Color colors[USE_CASE_COUNT];
    
    int load( std::string filename );
};

};

#endif // OPENAV_AVTK_THEME_HXX


#ifndef OPENAV_AVTK_THEME_HXX
#define OPENAV_AVTK_THEME_HXX

namespace Avtk
{

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
    Theme( std::string name = "OpenAV Default" ) :
      cornerRadius_( 1 ),
      lineWidthThin_( 0.9 ),
      lineWidthWide_( 2.1 )
    {}
    
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
    float useDefaultColor( cairo_t* cr, USE_CASE uc, float alpha_ )
    {
      switch( uc )
      {
        case BG:
          cairo_set_source_rgba(cr,  34/255.,  34/255.,  34/255., alpha_);
          break;
        case BG_DARK:
          cairo_set_source_rgba(cr,  17/255.,  17/255.,  17/255., alpha_);
          break;
      
        case FG:
          cairo_set_source_rgba(cr,  76/255.,  80/255.,  83/255., alpha_);
          break;
        case FG_DARK:
          cairo_set_source_rgba(cr,  35/255.,  87/255., 136/255., alpha_);
          break;
        
        case HIGHLIGHT:
          cairo_set_source_rgba(cr,   0/255., 128/255., 255/255., alpha_);
          break;
        
        case CORNER_RADIUS:
          return cornerRadius_;
          break;
        
        default:
          printf("Theme::useDefaultColor() color %i not handled!\n");
          break;
      };
    }
    
    virtual void cornerRadius( int c )
    {
      cornerRadius_ = c;
      //ui->redraw();
    }
    
    int cornerRadius_;
    int lineWidthThin_;
    int lineWidthWide_;
};

};

#endif // OPENAV_AVTK_THEME_HXX

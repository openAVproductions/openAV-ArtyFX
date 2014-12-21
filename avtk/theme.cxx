
#include "theme.hxx"

#include "ui.hxx"
#include "widget.hxx"

#include <fstream>
#include <sstream>
#include <fstream>
#include <iostream>

#include "picojson.hxx"

namespace Avtk
{

int Theme::privateID = 0;

Theme::Theme( Avtk::UI* ui_, std::string filename ) :
  ui( ui_ ),
  ID( privateID++ ),
  cornerRadius_( 1 ),
  lineWidthThin_( 0.9 ),
  lineWidthNorm_( 1.1 ),
  lineWidthWide_( 2.1 )
{
  int loadError = true;
  if( strlen( filename.c_str() ) > 0 )
  {
    loadError = load( filename );
  }
  if( loadError )
  {
    // set default values to the colors array
    colors[BG].c[0] = colors[BG].c[1] = colors[BG].c[2] = 34;
    colors[BG_DARK].c[0] = colors[BG_DARK].c[1] = colors[BG_DARK].c[2] = 17;
    
    colors[FG].c[0] = 76;
    colors[FG].c[1] = 80;
    colors[FG].c[2] = 83;
    
    colors[FG_DARK].c[0] = 35;
    colors[FG_DARK].c[1] = 87;
    colors[FG_DARK].c[2] =136;
    
    colors[HIGHLIGHT].c[0] =  0;
    colors[HIGHLIGHT].c[1] =128;
    colors[HIGHLIGHT].c[2] =255;
  }
}

int Theme::load( std::string filename )
{
  try
  {
    std::ifstream ifs;
    ifs.open ( filename.c_str(), std::ifstream::in);
    
    picojson::value v;
    ifs >> v;
    
    if( ifs.fail() )
    {
      printf("Theme::load() %s : File doesn't exist, abort.\n", filename.c_str() );
      //std::cerr << picojson::get_last_error() << std::endl;
      return -1;
    }
    
    const char* items[5] = 
    {
      "bg",
      "bg-dark",
      "fg",
      "fg-dark",
      "highlight"
    };
    
    for( int i = 0; i < 5; i++ )
    {
      // extract the 3 ints from the array, and store into Color array
      int colNum = 0;
      picojson::array list = v.get( items[i] ).get<picojson::array>();
      for (picojson::array::iterator iter = list.begin(); iter != list.end(); ++iter)
      {
        int tmp = (int)(*iter).get("c").get<double>();
        //printf("%s = %lf\r\n", items[i], tmp );
        colors[i].c[colNum++] = tmp;
      }
    }
  }
  catch( ... )
  {
    printf("Theme::load() Error loading theme from %s : falling back to default.Double check file-exists and JSON contents valid.\n", filename.c_str() );
    // *any* error, and we don't use the theme
    return -1;
  }
  
  // successful load
  return 0;
}

void Theme::cornerRadius( int c )
{
  cornerRadius_ = c;
  ui->redraw();
}

float Theme::color( cairo_t* cr, USE_CASE uc, float alpha_ )
{
  float r = colors[uc].c[0] / 255.;
  float g = colors[uc].c[1] / 255.;
  float b = colors[uc].c[2] / 255.;
  //printf("%f, %f, %f\n", r, g, b );
  cairo_set_source_rgba(cr, r, g, b, alpha_);
  return 0;
}

}; // Avtk

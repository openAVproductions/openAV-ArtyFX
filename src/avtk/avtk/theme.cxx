
#include "theme.hxx"

#include "common.hxx"

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
 	/* 
  if( strlen( filename.c_str() ) > 0 )
  {
    std::ifstream ifs;
    ifs.open ( filename.c_str(), std::ifstream::in);
    
    if( ifs.fail() )
    {
      printf("Theme::load() %s : File doesn't exist, using default theme instead.\n", filename.c_str() );
    }
    else
    {
      std::stringstream buffer;
      buffer << ifs.rdbuf();
      
      loadError = load( buffer.str() );
      
      printf("Theme() load() returns %i\n", loadError );
    }
  }
  else
  {
    printf("Avtk:Theme() filename is 0 chars..\n");
  }
 	*/
	
  if( loadError )
  {
    printf("Theme::Theme() Load error: using hard coded defaults\n" );
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

int Theme::load( std::string jsonTheme )
{
  printf("%s : jsonTheme = %s\n", __FUNCTION__, jsonTheme.c_str() );
  
  try
  {
    std::ifstream ifs;
    ifs.open ( "green.avtk", std::ifstream::in);
    
    picojson::value v;
    ifs >> v;
    
    
    const char* items[5] = 
    {
      "bg",
      "bg-dark",
      "fg",
      "fg-dark",
      "highlight"
    };
    
    printf("value ok\n");
    
    for( int i = 0; i < 5; i++ )
    {
      // extract the 3 ints from the array, and store into Color array
      int colNum = 0;
      
      if( !v.is<picojson::object>() )
      {
        printf("Error: v is NOT array\n");
        return -1;
      }
      
      //else
      {
        picojson::array list = v.get( items[i] ).get<picojson::array>();
        printf("array list ok\n");
        
        //std::cerr << picojson::get_last_error() << std::endl;
        
        for (picojson::array::iterator iter = list.begin(); iter != list.end(); ++iter)
        {
          double tmp = (int)(*iter).get("c").get<double>();
          printf("%s = %lf\r\n", items[i], tmp );
          colors[i].c[colNum++] = tmp;
        }
      }
    }
  }
  catch( ... )
  {
    printf("Theme::load() Error loading theme from %s : falling back to default.Double check file-exists and JSON contents valid.\n", jsonTheme.c_str() );
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

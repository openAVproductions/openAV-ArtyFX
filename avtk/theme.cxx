
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
  if( strlen( filename.c_str() ) > 0 )
    load( filename );
}

void Theme::load( std::string filename )
{
  try
  {
    std::ifstream ifs;
    ifs.open ( filename.c_str(), std::ifstream::in);
    
    picojson::value v;
    ifs >> v;
    
    if( ifs.fail() )
    {
      std::cerr << picojson::get_last_error() << std::endl;
      return;
    }
    
    // extract the 3 ints from the array
    picojson::array list = v.get("highlight").get<picojson::array>();
    for (picojson::array::iterator iter = list.begin(); iter != list.end(); ++iter)
    {
      double tmp = (*iter).get("c").get<double>();
      printf("values = %lf\r\n", tmp );
    }
  }
  catch( ... )
  {
    printf("Theme::load() Error loading theme from %s : falling back to default.Double check file-exists and JSON contents valid.\n", filename.c_str() );
  }
  
}

void Theme::cornerRadius( int c )
{
  cornerRadius_ = c;
  ui->redraw();
  printf("corner = %i\n", c );
}

float Theme::useDefaultColor( cairo_t* cr, USE_CASE uc, float alpha_ )
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

}; // Avtk

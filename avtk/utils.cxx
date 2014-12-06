
#include "utils.hxx"

#include "avtk.hxx"

namespace Avtk
{

int directoryContents( std::string d, std::vector< std::string >& files, bool printErrors )
{
  tinydir_dir dir;
  if (tinydir_open(&dir, d.c_str() ) == -1)
  {
    if( printErrors )
      printf("Error opening dir %s", d.c_str() );
    tinydir_close(&dir);
    return OPENAV_ERROR;
  }
  
  while (dir.has_next)
  {
    tinydir_file file;
    if (tinydir_readfile(&dir, &file) == -1)
    {
      if( printErrors )
        printf("Error getting file from dir %s\n", d.c_str() );
      return OPENAV_ERROR;
    }
    
    if ( !file.is_dir )
    {
      std::stringstream s;
      s << d << "/" << file.name;
      files.push_back( s.str() );
    }
    
    tinydir_next(&dir);
  }
  
  return OPENAV_OK;
}


} //  Avtk namespace

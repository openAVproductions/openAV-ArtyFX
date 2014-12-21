
#include "utils.hxx"

#include "avtk.hxx"
#include "theme.hxx"
#include <sndfile.h>

namespace Avtk
{
int loadSample( std::string path, std::vector< float >& sample, bool printErrors )
{
#ifdef AVTK_SNDFILE
  SF_INFO info;
  memset( &info, 0, sizeof( SF_INFO ) );
  SNDFILE* const sndfile = sf_open( path.c_str(), SFM_READ, &info);
  if ( !sndfile )
  {
    printf("Failed to open sample '%s'\n", path.c_str() );
    return -1;
  }
  
  if( !(info.channels == 1 || info.channels == 2) )
  {
    int chnls = info.channels;
    printf("Loading sample %s, channels = %i\n", path.c_str(), chnls );
    return -1;
  }
  
  sample.resize( info.frames * info.channels );
  
  sf_seek(sndfile, 0ul, SEEK_SET);
  sf_read_float( sndfile, &sample[0], info.frames * info.channels );
  sf_close(sndfile);
  
  return OPENAV_OK;
#else
  if( printErrors )
  {
    printf("AVTK compiled without SNDFILE support: cannot load audio sample.\n");
  }
  return OPENAV_ERROR;
#endif
}


int directories( std::string d, std::vector< std::string >& files, bool nameOnly, bool printErrors )
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
    
    if ( file.is_dir )
    {
      if ( nameOnly )
      {
        files.push_back( file.name );
      }
      else
      {
        std::stringstream s;
        s << d << "/" << file.name;
        files.push_back( s.str() );
      }
    }
    
    tinydir_next(&dir);
  }
  
  return OPENAV_OK;
}

int directoryContents( std::string d, std::vector< std::string >& files, bool nameOnly, bool printErrors )
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
      if ( nameOnly )
      {
        files.push_back( file.name );
      }
      else
      {
        std::stringstream s;
        s << d << "/" << file.name;
        files.push_back( s.str() );
      }
    }
    
    tinydir_next(&dir);
  }
  
  return OPENAV_OK;
}


} //  Avtk namespace

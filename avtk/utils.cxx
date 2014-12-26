
#include "utils.hxx"

#include "avtk.hxx"
#include "theme.hxx"
#include <algorithm>
#include <cstring>
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
      // no . or ..
      if( strcmp(file.name, "..") != 0 && strcmp( "." ,file.name) != 0 )
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
    }
    
    tinydir_next(&dir);
  }
  
  // sort them alphabetically
  std::sort( files.begin(), files.end() );
  
  return OPENAV_OK;
}

int directoryContents( std::string d, std::vector< std::string >& files, std::string& strippedFilenameStart, bool nameOnly, bool smartShortStrings, bool printErrors )
{
  files.clear();
  
  tinydir_dir dir;
  if (tinydir_open(&dir, d.c_str() ) == -1)
  {
    if( printErrors )
      printf("Error opening dir %s", d.c_str() );
    tinydir_close(&dir);
    return OPENAV_ERROR;
  }
  
  // if we have the full path, don't smart-remove the path!
  if( !nameOnly )
  {
    smartShortStrings = false;
  }
  
  // for smartShortStrings, we keep the shortest common string from a directory,
  // and take those characters away from each item: providing a neat listing.
  std::string commonStart;
  int nCharSame = 0;
  bool tryCommonStart = true;
  
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
        if( tryCommonStart && commonStart.size() == 0 )
        {
#ifdef AVTK_DEBUG
          printf("commonStart init %s\n", file.name );
#endif
          commonStart = file.name;
          nCharSame = commonStart.size();
        }
        else if( tryCommonStart )
        {
          // compare with commonStart, and find common N characters
          int maxLen = strlen( commonStart.c_str() );
          if( strlen( file.name ) <= maxLen )
            maxLen = strlen( file.name );
          
          if( maxLen > nCharSame )
            maxLen = nCharSame;
          
          for(int i = 0; i < maxLen; i++ )
          {
            if( commonStart[i] != file.name[i] )
            {
#ifdef AVTK_DEBUG
              printf("char # %i is not equal!\n", i );
#endif
              nCharSame = i;
              break;
            }
          }
          
          if( nCharSame == 0 )
          {
            tryCommonStart = false;
          }
          else
          {
            commonStart = commonStart.substr( 0, nCharSame );
#ifdef AVTK_DEBUG
            printf("Common chars = %i, %s\n", nCharSame, commonStart.c_str() );
#endif
          }
        }
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
  
  /// if smartShortStrings, we strip the starting nCharSame from every name
  // TODO: if there's only 1 file, or two files who's filenames totally match 
  // except for the final part of the extension (eg foo.wav foo.wav.bak) don't 
  // remove the whole foo.wav string: it essentially hides the file from the user!
  if( smartShortStrings )
  {
    strippedFilenameStart = commonStart;
    
    for(int i = 0; i < files.size(); i++ )
    {
      // remove common string
      int fSize = files.at(i).size();
      std::string tmp = files.at(i).substr( nCharSame );
      
      files.at(i) = tmp;
      
      /* We would need to remember *every* file's extension individually:
          its possible, but demands an Avtk::File class or such to handle well
      // remove dot extension from file, eg: ".wav"
      int dotPos = files.at(i).rfind(".");
      if( dotPos != std::string::npos ) 
      {
        files.at(i) = files.at(i).substr( 0, dotPos );
        printf("dotPos of %s = %i\n", files.at(i).c_str(), dotPos );
      }
      */
#ifdef AVTK_DEBUG
      printf("i : %s\n", files.at(i).c_str() );
#endif
    }
  }
  
  // sort them alphabetically
  std::sort( files.begin(), files.end() );
  
  return OPENAV_OK;
}


} //  Avtk namespace

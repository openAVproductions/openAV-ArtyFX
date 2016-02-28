
#ifndef OPENAV_AVTK_UTILS_HXX
#define OPENAV_AVTK_UTILS_HXX

/** Utils
 * Utils provides a variety of utility functions that are cross-platform.
 * They're based on small libraries like tinydir and zix, and aim to serve
 * creating fully features UI's in a lightweight and cross platform way.
**/

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <libgen.h>
#include <sys/stat.h>

#include "tinydir.hxx"

namespace Avtk
{
int fileUpLevel( std::string path, std::string& newPath );

int loadSample( std::string path, std::vector< float >& sample, bool printErrors = true );
int directories( std::string d, std::vector< std::string >& files, bool nameOnly = true, bool printErrors = true );

/** lists the contents of a directory @param directory, and stores the
 * resuliting filenames in the provided @param output vector. The options are
 * provided to allow for better presentation of the contents:<br/>
 * - @param nameOnly (provides only file *name* without path).<br/>
 * - @param smartShortStrings (remove starting N characters if common in all files).<br/>
 * - @param printErrors (prints errors if file doesn't exist etc).
 */
int directoryContents(  std::string directory,
                        std::vector< std::string >& output,
                        std::string& strippedFilenameStart,
                        bool nameOnly = true,
                        bool smartShortStrings = true,
                        bool printErrors = true );
};

#endif // OPENAV_AVTK_UTILS_HXX


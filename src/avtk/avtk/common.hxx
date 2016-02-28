/*
 * Author: Harry van Haaren 2015
 *         harryhaaren@gmail.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OPENAV_AVTK_COMMON_HXX
#define OPENAV_AVTK_COMMON_HXX

#include <stdarg.h>

#ifndef OPENAV_NAME
#define NAME "Avtk"
#endif

enum DEBUG_LEVEL {
	DEBUG_LEVEL_DEVELOPER = 0, // on #ifdef AVTK_DEBUG at compile time only
	DEBUG_LEVEL_NOTE,
	DEBUG_LEVEL_WARN,
	DEBUG_LEVEL_ERROR,
};

void avtk_debug( int warnLevel, const char* name, const char* file, const char* func, int line,
                 const char* format = 0, ... );

#ifdef AVTK_DEBUG
#define AVTK_DEV( format, args... ) avtk_debug( DEBUG_LEVEL_DEVELOPER, NAME, __FILE__, __FUNCTION__, __LINE__, format, ## args )
#else
#define AVTK_DEV( format, args... )
#endif

#define AVTK_NOTE( format, args... ) avtk_debug( DEBUG_LEVEL_NOTE, NAME, __FILE__, __FUNCTION__, __LINE__, format, ## args )
#define AVTK_WARN( format, args... ) avtk_debug( DEBUG_LEVEL_WARN, NAME, __FILE__, __FUNCTION__, __LINE__, format, ## args )
#define AVTK_ERROR( format, args... ) avtk_debug( DEBUG_LEVEL_ERROR, NAME, __FILE__, __FUNCTION__, __LINE__, format, ## args )

#endif // OPENAV_AVTK_COMMON_HXX


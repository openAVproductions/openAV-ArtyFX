
cmake_minimum_required (VERSION 2.6)

project (AVTK)

set(AVTK_VERSION_MAJOR "0")
set(AVTK_VERSION_MINOR "0")
set(AVTK_VERSION_PATCH "1")

set(AVTK_VERSION "${AVTK_VERSION_MAJOR}.${AVTK_VERSION_MINOR}.${AVTK_VERSION_PATCH}")

option(AVTK_RELEASE_BUILD   "Build for releasing"         OFF )
option(AVTK_DEBUG_PRINTS    "Build with debugging prints" ON )
option(AVTK_SNDFILE_SUPPORT "Build with sndfile library"  ON )


################################################################################
### Code compilation starts here
################################################################################

ADD_DEFINITIONS( "-DAVTK_VERSION_STRING=\"v ${AVTK_VERSION}\"" )

include_directories ("${PROJECT_SOURCE_DIR}/avtk/")

find_package(PkgConfig)

IF(AVTK_DEBUG_PRINTS)
  ADD_DEFINITIONS( "-DAVTK_DEBUG" )
  #ADD_DEFINITIONS( "-DAVTK_DEBUG_DTOR" )
ENDIF()

IF( AVTK_SNDFILE_SUPPORT )
  ADD_DEFINITIONS( "-DAVTK_SNDFILE" )
  pkg_check_modules(SNDFILE sndfile REQUIRED)
  include_directories( ${SNDFILE_INCLUDE_DIRS})
  link_directories   ( ${SNDFILE_LIBRARY_DIRS})
ENDIF()

ADD_DEFINITIONS( "-DPUGL_HAVE_CAIRO" )
pkg_check_modules(CAIRO cairo REQUIRED)
include_directories( ${CAIRO_INCLUDE_DIRS})
link_directories   ( ${CAIRO_LIBRARY_DIRS})

pkg_check_modules(X11 x11 REQUIRED)
include_directories( ${X11_INCLUDE_DIRS} )
link_directories   ( ${X11_LIBRARY_DIRS} )

SET(CMAKE_CXX_FLAGS "-trigraphs -g")

FILE(GLOB src main.cxx test_ui.cxx avtk/*.cxx avtk/pugl/pugl_x11.c )
ADD_EXECUTABLE( avtk ${src} )

#target_link_libraries( avtk ${GL_LIBRARIES}     )
target_link_libraries( avtk ${CAIRO_LIBRARIES}  )
target_link_libraries( avtk ${X11_LIBRARIES}    )

IF( AVTK_SNDFILE_SUPPORT )
  target_link_libraries( avtk ${SNDFILE_LIBRARIES}    )
ENDIF()

configure_file( "avtk/themes/default.avtk"  "default.avtk" COPYONLY)
configure_file( "avtk/themes/green.avtk"    "green.avtk"   COPYONLY)
configure_file( "avtk/themes/orange.avtk"   "orange.avtk"  COPYONLY)
configure_file( "avtk/themes/yellow.avtk"   "yellow.avtk"  COPYONLY)



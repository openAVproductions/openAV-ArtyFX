
cmake_minimum_required (VERSION 2.6)

project (artyfx)

set(ARTYFX_VERSION_MAJOR "1")
set(ARTYFX_VERSION_MINOR "2")
set(ARTYFX_VERSION_PATCH "0")

set(ARTYFX_VERSION "${ARTYFX_VERSION_MAJOR}.${ARTYFX_VERSION_MINOR}.${ARTYFX_VERSION_PATCH}")

option(RELEASE_BUILD  "Build for production usage" ON  )
option(HAVE_NTK       "Build GUI (NTK)"            ON  )

find_package(PkgConfig)

#SET(CMAKE_INSTALL_PREFIX "/usr" )

pkg_check_modules(LV2 lv2>=1.8.0 REQUIRED)
include_directories( ${LV2_INCLUDE_DIRS}  )

IF(HAVE_NTK)
  ADD_DEFINITIONS(-DOPENAV_USE_NTK)
  pkg_check_modules(NTK ntk REQUIRED)
  include_directories( ${NTK_INCLUDE_DIRS}  )
  link_directories   ( ${NTK_LIBRARY_DIRS}  )
ENDIF()

SET(CMAKE_SHARED_LINKER_FLAGS "-fPIC -shared -Wl,-z,nodelete -Wl,--no-undefined")

IF(RELEASE_BUILD)
  SET(CMAKE_CXX_FLAGS "-g -Wall -Wno-unused-variable -msse2 -mfpmath=sse -ffast-math")
  SET(CMAKE_C_FLAGS "-g -Wall -W -Wno-unused-variable -msse2 -mfpmath=sse -ffast-math")
ENDIF()

FILE(GLOB sources
  artyfx.cxx +
  bitta/dsp/*.cxx 
  della/dsp/*.cxx 
  driva/dsp/*.cxx + driva/dsp/rr/*.cxx 
  ducka/dsp/*.cxx 
  filta/dsp/*.cxx 
  friza/dsp/*.cxx 
  kuiza/dsp/*.cxx + kuiza/dsp/eq/*.cc
  masha/dsp/*.cxx 
  panda/dsp/*.cxx 
  roomy/dsp/*.cxx 
  satma/dsp/*.cxx 
  vihda/dsp/*.cxx 
  whaaa/dsp/*.cxx 
  )

# don't compile UI files if we don't have NTK
IF(HAVE_NTK)
  FILE(GLOB sources
    ${sources} +
    artyfx_ui.cxx +
    bitta/gui/*.cxx +
    della/gui/*.cxx +
    ducka/gui/*.cxx +
    filta/gui/*.cxx +
    kuiza/gui/*.cxx +
    masha/gui/*.cxx +
    panda/gui/*.cxx +
    roomy/gui/*.cxx +
    satma/gui/*.cxx +
    vihda/gui/*.cxx
    whaaa/gui/*.cxx
     )
ENDIF()


string( ASCII 27 _esc)

function(COLOR_MESSAGE TEXT)
    if(CMAKE_COLOR_MAKEFILE)
        MESSAGE(${TEXT})
    else()
        string(REGEX REPLACE "${_esc}.[0123456789;]*m" "" __TEXT ${TEXT} )
        MESSAGE(${__TEXT})
    endif()
endfunction()

SET(green     "${_esc}[1;32m")
SET(blue      "${_esc}[1;34m")
SET(reset     "${_esc}[1;0m")

COLOR_MESSAGE( "
  ${blue}ArtyFX Configuration : ${ARTYFX_VERSION}${reset}
-----------------------
* ${green}Install Directory${reset}               : ${CMAKE_INSTALL_PREFIX}
* ${green}GUI (ntk)${reset}                       : ${HAVE_NTK}
")

ADD_LIBRARY(artyfx SHARED ${sources} )

IF(HAVE_NTK)
target_link_libraries( artyfx ${NTK_LIBRARIES} )
ENDIF()

# Remove "lib" part before name (artyfx.so, not libartyfx.so)
set_target_properties(artyfx PROPERTIES PREFIX "")

# add the install targets
install (TARGETS artyfx DESTINATION lib/lv2/artyfx.lv2/)

FILE(GLOB ttls "artyfx.lv2/*")
install(FILES ${ttls} DESTINATION lib/lv2/artyfx.lv2/)


cmake_minimum_required (VERSION 2.6)

project (artyfx)

set(ARTYFX_VERSION_MAJOR "1")
set(ARTYFX_VERSION_MINOR "2")
set(ARTYFX_VERSION_PATCH "0")

set(ARTYFX_VERSION "${ARTYFX_VERSION_MAJOR}.${ARTYFX_VERSION_MINOR}.${ARTYFX_VERSION_PATCH}")

option(RELEASE_BUILD  "Build for production usage" ON )

find_package(PkgConfig)

#SET(CMAKE_INSTALL_PREFIX "/usr" )

pkg_check_modules(LV2 lv2 REQUIRED)
include_directories( ${LV2_INCLUDE_DIRS}  )

pkg_check_modules(NTK ntk REQUIRED)
include_directories( ${NTK_INCLUDE_DIRS}  )
link_directories   ( ${NTK_LIBRARY_DIRS}  )

SET(CMAKE_SHARED_LINKER_FLAGS "-fPIC -shared -Wl,-z,nodelete -Wl,--no-undefined")

IF(RELEASE_BUILD)
  SET(CMAKE_CXX_FLAGS "-g -Wall -Wno-unused-variable -msse2 -mfpmath=sse -ffast-math")
  SET(CMAKE_C_FLAGS "-g -Wall -W -Wno-unused-variable -msse2 -mfpmath=sse -ffast-math")
ENDIF()

FILE(GLOB sources
  *.cxx +
  bitta/dsp/*.cxx bitta/gui/*.cxx +
  della/dsp/*.cxx della/gui/*.cxx +
  ducka/dsp/*.cxx ducka/gui/*.cxx +
  filta/dsp/*.cxx filta/gui/*.cxx +
  kuiza/dsp/*.cxx kuiza/dsp/eq/*.cc kuiza/gui/*.cxx +
  masha/dsp/*.cxx masha/gui/*.cxx +
  panda/dsp/*.cxx panda/gui/*.cxx +
  roomy/dsp/*.cxx roomy/gui/*.cxx +
  satma/dsp/*.cxx satma/gui/*.cxx +
  vihda/dsp/*.cxx vihda/gui/*.cxx )

ADD_LIBRARY(artyfx SHARED ${sources})

target_link_libraries( artyfx ${NTK_LIBRARIES} )

# Remove "lib" part before name (artyfx.so, not libartyfx.so)
set_target_properties(artyfx PROPERTIES PREFIX "")

# add the install targets
install (TARGETS artyfx DESTINATION lib/lv2/artyfx.lv2/)

FILE(GLOB ttls "artyfx.lv2/*")
install(FILES ${ttls} DESTINATION lib/lv2/artyfx.lv2/)

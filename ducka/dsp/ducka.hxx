
#ifndef OPENAV_DUCKA
#define OPENAV_DUCKA

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define DUCKA_URI    "http://www.openavproductions.com/artyfx#ducka"
#define DUCKA_UI_URI "http://www.openavproductions.com/artyfx#ducka/gui"

typedef enum
{
  DUCKA_INPUT_L = 0,
  DUCKA_INPUT_R,
  
  DUCKA_SIDECHAIN,
  
  DUCKA_OUTPUT_L,
  DUCKA_OUTPUT_R,
  
  
  DUCKA_THRESHOLD,
  DUCKA_TARGET,
  DUCKA_REDUCTION,
  
  DUCKA_RELEASE_TIME,
} PortIndex;

#endif // OPENAV_DUCKA

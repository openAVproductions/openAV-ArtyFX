
#ifndef OPENAV_MASHA
#define OPENAV_MASHA

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define MASHA_URI    "http://www.openavproductions.com/artyfx#masha"
#define MASHA_UI_URI "http://www.openavproductions.com/artyfx#masha/gui"

typedef enum
{
  MASHA_INPUT_L = 0,
  MASHA_INPUT_R,
  
  MASHA_OUTPUT_L,
  MASHA_OUTPUT_R,
  
  MASHA_TIME,
  MASHA_DAMPING,
  MASHA_DRY_WET,
} PortIndex;

#endif // OPENAV_MASHA

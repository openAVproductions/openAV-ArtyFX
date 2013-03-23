
#ifndef OPENAV_CUTOFF
#define OPENAV_CUTOFF

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define CUTOFF_URI    "http://www.openavproductions.com/cutoff"
#define CUTOFF_UI_URI "http://www.openavproductions.com/cutoff/gui"

typedef enum {
  CUTOFF_INPUT = 0,
  CUTOFF_OUTPUT,
  CUTOFF_TYPE,
  CUTOFF_FREQ,
  CUTOFF_GAIN,
  CUTOFF_Q,
} PortIndex;

#endif // OPENAV_CUTOFF

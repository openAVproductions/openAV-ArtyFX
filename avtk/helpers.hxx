
#ifndef OPENAV_AVTK_HELPERS_HXX
#define OPENAV_AVTK_HELPERS_HXX

#include <cairo/cairo.h>

// code aquisited from pugl_cairo_test.c, author David Robillard.
void roundedBox(cairo_t* cr,
    double x, double y,
    double w, double h,
    double radius );

#endif // OPENAV_AVTK_HELPERS_HXX

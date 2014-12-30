
#ifndef OPENAV_AVTK_SCROLL_HXX
#define OPENAV_AVTK_SCROLL_HXX

#include "group.hxx"

#include <string>
#include <vector>

// for the childs cairo_t
#include <cairo/cairo.h>

namespace Avtk
{

/** Scroll
 * A widget that wraps a group, and functions as a scrollable area. This seems
 * a simple task, however in order to appropriately and quickly draw the entire
 * child widget on scroll-movements, this becomes non-trivial.
 * 
 * The solution is to have a local cached cairo_t, which the child draws to.
 * The cairo_t context for the child is matched to the childs size, and scales
 * up and down with the child widget.
 * 
 * The actual drawing of the child cairo_t is done in Scroll::draw(), where the
 * co-ordinates of the child cairo_t are moved around on paint().
 */
class Scroll : public Group
{
  public:
    Scroll( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
    
    virtual void draw( cairo_t* cr );
    
    /// sets widget to be the child of this scroll. Only one widget can be inside
    /// a scroll at a time: add a group if multiple child-widgets need to scroll
    void set( Widget* child );
  
  protected:
    bool newChildCr;
    cairo_t* childCr;
};

};

#endif // OPENAV_AVTK_SCROLL_HXX

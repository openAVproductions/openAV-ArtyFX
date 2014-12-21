
#ifndef OPENAV_AVTK_GROUP_HXX
#define OPENAV_AVTK_GROUP_HXX

#include "widget.hxx"

namespace Avtk
{

class UI;

/** Group
 * The Avtk::Group class is a container for widgets. The group ensures that the
 * child widgets parent() pointers are valid, and handles re-sizing.
 * 
 * The virtual destructor allows deriving from Group to create custom layout
 * widgets, or offer features not available with standard the Group.
 * 
 * On add(), the Group sets the child widgets parent pointer to this.
 * On remove( Widget* child ), the child's parent() pointer is set to 0.
 * 
 * // TODO how resize works here
 * 
 */
class Group : public Widget
{
  public:
    Group( Avtk::UI* ui, int x, int y, int w, int h, std::string label );
    virtual ~Group();
    
    /// adds a Widget to this group: its parent pointer is set to this.
    void add    ( Widget* child );
    
    /// removes a Widget from this group: its parent pointer is set to 0.
    //void remove ( Widget* child );
    
    virtual void clear();
    
    void draw( cairo_t* cr );
    
    /// should the group all be the same width / height
    enum GROUP_MODE {
      NONE,
      WIDTH_EQUAL,
      HEIGHT_EQUAL,
    };
    
    void mode( GROUP_MODE gm );
    
    /// virtual so it can be overriden by List and other widgets that want to
    /// intercept callbacks from a range of widgets
    virtual void valueCB( Widget* w );
  
  private:
    std::vector< Widget* > children;
    
    GROUP_MODE groupMode;
    
    static void staticGroupCB( Widget* w, void* ud )
    {
      Group* g = (Group*)ud;
      g->valueCB( w );
    }
};

};

#endif // OPENAV_AVTK_GROUP_HXX

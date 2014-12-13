
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
    void remove ( Widget* child );
    
    /// should the group all be the same width / height
    enum GROUP_MODE {
      GROUP_NONE,
      GROUP_WIDTH_EQUAL,
      GROUP_HEIGHT_EQUAL,
    };
    
    void mode( GROUP_MODE gm );
  
  private:
    std::vector< Widget* > children;
    
    GROUP_MODE groupMode;
};

};

#endif // OPENAV_AVTK_GROUP_HXX

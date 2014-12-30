
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
    
    /// mark all children too
    virtual void visible( bool visibile );
    /// only returns true if *all* children are visible
    virtual bool visible();
    
    /// adds a Widget to this group: its parent pointer is set to this.
    void add    ( Widget* child );
    
    /// removes a Widget from this group: its parent pointer is set to 0.
    void remove ( Widget* child );
    
    virtual int x(){return Widget::x();}
    virtual int y(){return Widget::y();}
    virtual int w(){return Widget::w();}
    virtual int h(){return Widget::h();}
    
    virtual void x(int x);
    virtual void y(int y);
    virtual void w(int w);
    virtual void h(int h);
    
    /// handles an event, propagating it to all children
    virtual int handle( const PuglEvent* event );
    
    /// when a child resizes itself, it calls the parents resize to inform it.
    /// The Widget* is of the child that wishes to resize itself
    virtual void resizeNotify( Widget* w );
    
    /// clears all child widgets
    virtual void clear();
    
    /// draws the contents of the group
    void draw( cairo_t* cr );
    
    /// sets the space between widgets
    void spacing( int s ) { spacing_ = s; }
    
    /// should the group all be the same width / height
    enum GROUP_MODE {
      NONE,
      WIDTH_EQUAL,
      HEIGHT_EQUAL,
    };
    
    void mode( GROUP_MODE gm );
    
    enum GROUP_VALUE {
      VALUE_NORMAL,
      VALUE_SINGLE_CHILD,
    };
    
    void valueMode( GROUP_VALUE gv )
    {
      valueMode_ = gv;
    }
    
    /// virtual so it can be overriden by List and other widgets that want to
    /// intercept callbacks from a range of widgets
    virtual void valueCB( Widget* w );
  
  protected:
    /// constructor for top-level UI only
    Group( Avtk::UI* ui );
    
    std::vector< Widget* > children;
    
    int spacing_;
    
    GROUP_MODE  groupMode;
    GROUP_VALUE valueMode_;
    
    static void staticGroupCB( Widget* w, void* ud )
    {
      Group* g = (Group*)ud;
      g->valueCB( w );
    }
};

};

#endif // OPENAV_AVTK_GROUP_HXX

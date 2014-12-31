
#ifndef OPENAV_AVTK_LIST_HXX
#define OPENAV_AVTK_LIST_HXX

#include "group.hxx"

#include <string>
#include <vector>

namespace Avtk
{

/** List
 * A widget that wraps a group, and displays a list of strings as a neat choice
 * list box. ListItem widgets are placed inside, but other widgets can be added
 * too: a ListItem is a normal AVTK Widget.
 */
class List : public Group
{
  public:
    List( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
    virtual ~List(){}
    virtual void draw( cairo_t* cr );
    
    void addItem( std::string newItem );
    
    /// integer input for which item to highlight
    void value( float v );
    
    void show( std::vector< std::string > data );
    
    virtual void clear();
    
    std::string selectedString();
  
  protected:
    std::vector< std::string > items;
    
    int lastClickedItem;
    virtual void valueCB( Widget* w );
};

};

#endif // OPENAV_AVTK_LIST_HXX

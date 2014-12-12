
#ifndef OPENAV_AVTK_LIST_HXX
#define OPENAV_AVTK_LIST_HXX

#include "widget.hxx"

#include <string>
#include <vector>

namespace Avtk
{

class List : public Widget
{
  public:
    List( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
    
    virtual void draw( cairo_t* cr );
    
    void show( std::vector< std::string > data );
    
    int  selectItem();
    void selectItem( int select );
  
  protected:
    std::vector< std::string > items;
};

};

#endif // OPENAV_AVTK_LIST_HXX

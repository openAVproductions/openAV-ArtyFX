
#ifndef OPENAV_AVTK_LISTITEM_HXX
#define OPENAV_AVTK_LISTITEM_HXX

#include "widget.hxx"

namespace Avtk
{

class ListItem : public Widget
{
  public:
    ListItem( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
    virtual ~ListItem(){}
    virtual void draw( cairo_t* cr );
};

};

#endif // OPENAV_AVTK_LISTITEM_HXX

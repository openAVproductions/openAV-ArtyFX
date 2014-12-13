
#include "group.hxx"

namespace Avtk
{

Group::Group( Avtk::UI* ui, int x, int y, int w, int h, std::string label ) :
  Widget( ui, x, y, w, h, label ),
  groupMode( GROUP_NONE )
{
  
}

void Group::add( Widget* child )
{
  children.push_back( child );
  child->parent( this );
}

void Group::mode( GROUP_MODE gm )
{
}

Group::~Group()
{
}

};

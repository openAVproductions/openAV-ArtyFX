
#ifndef OPENAV_ARTYFX_ROOMY_UI_HXX
#define OPENAV_ARTYFX_ROOMY_UI_HXX

#include "../avtk/avtk.hxx"

namespace Avtk
{
  class Widget;
};

class RoomyUI : public Avtk::UI
{
  public:
    RoomyUI(PuglNativeWindow parent = 0);
    virtual ~RoomyUI(){}
    
    // ignore widget values, empty function body
    virtual void widgetValueCB( Avtk::Widget* widget){}
    
    Avtk::Dial* dial1;
    Avtk::Dial* dial2;
    Avtk::Dial* dial3;
};


#endif // OPENAV_AVTK_DIAL_TEST_HXX

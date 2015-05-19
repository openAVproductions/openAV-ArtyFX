
#ifndef OPENAV_ARTYFX_ROOMY_UI_HXX
#define OPENAV_ARTYFX_ROOMY_UI_HXX

#include "../avtk/avtk.hxx"

namespace Avtk
{
  class Widget;
  class Reverb;
};

class RoomyUI : public Avtk::UI
{
  public:
    RoomyUI(PuglNativeWindow parent = 0);
    virtual ~RoomyUI(){}
    
    // ignore widget values, empty function body
    virtual void widgetValueCB( Avtk::Widget* widget);
    
    virtual void lv2PortEvent(  uint32_t index,
                                uint32_t buffer_size,
                                uint32_t format, 
                                const void* buffer );

    Avtk::Reverb* rev;
    Avtk::Dial* dial1;
    Avtk::Dial* dial2;
    Avtk::Dial* dial3;
};


#endif // OPENAV_AVTK_DIAL_TEST_HXX

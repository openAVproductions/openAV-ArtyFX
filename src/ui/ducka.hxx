
#ifndef OPENAV_ARTYFX_DUCKA_UI_HXX
#define OPENAV_ARTYFX_DUCKA_UI_HXX

#include "avtk.hxx"

namespace Avtk
{
  class Widget;
  class Delay;
};

class DuckaUI : public Avtk::UI
{
  public:
    DuckaUI(PuglNativeWindow parent = 0);
    virtual ~DuckaUI(){}
    
    virtual void widgetValueCB( Avtk::Widget* widget);
    
    virtual void lv2PortEvent(  uint32_t index,
                                uint32_t buffer_size,
                                uint32_t format, 
                                const void* buffer );

    Avtk::Delay* rev;
    Avtk::Dial* dial1;
    Avtk::Dial* dial2;
    Avtk::Dial* dial3;
};


#endif // OPENAV_ARTYFX_DUCKA_UI_HXX

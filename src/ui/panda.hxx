
#ifndef OPENAV_ARTYFX_PANDA_UI_HXX
#define OPENAV_ARTYFX_PANDA_UI_HXX

#include "avtk.hxx"

namespace Avtk
{
  class Widget;
  class Delay;
};

class PandaUI : public Avtk::UI
{
  public:
    PandaUI(PuglNativeWindow parent = 0);
    virtual ~PandaUI(){}
    
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


#endif // OPENAV_ARTYFX_PANDA_UI_HXX

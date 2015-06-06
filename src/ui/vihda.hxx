
#ifndef OPENAV_ARTYFX_VIHDA_UI_HXX
#define OPENAV_ARTYFX_VIHDA_UI_HXX

#include "avtk.hxx"

namespace Avtk
{
  class Widget;
  class Delay;
};

class VihdaUI : public Avtk::UI
{
  public:
    VihdaUI(PuglNativeWindow parent = 0);
    virtual ~VihdaUI(){}
    
    virtual void widgetValueCB( Avtk::Widget* widget);
    
    virtual void lv2PortEvent(  uint32_t index,
                                uint32_t buffer_size,
                                uint32_t format, 
                                const void* buffer );

    Avtk::Delay* rev;
    Avtk::Dial* dial1;
    Avtk::Dial* dial2;
};


#endif // OPENAV_ARTYFX_VIHDA_UI_HXX

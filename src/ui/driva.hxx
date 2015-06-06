
#ifndef OPENAV_ARTYFX_DRIVA_UI_HXX
#define OPENAV_ARTYFX_DRIVA_UI_HXX

#include "avtk.hxx"

namespace Avtk
{
  class Widget;
  class Distortion;
};

class DrivaUI : public Avtk::UI
{
  public:
    DrivaUI(PuglNativeWindow parent = 0);
    virtual ~DrivaUI(){}
    
    virtual void widgetValueCB( Avtk::Widget* widget);
    
    virtual void lv2PortEvent(  uint32_t index,
                                uint32_t buffer_size,
                                uint32_t format, 
                                const void* buffer );

    Avtk::Distortion* graph;
    Avtk::Dial* dial1;
    Avtk::Dial* dial2;
};


#endif // OPENAV_ARTYFX_DRIVA_UI_HXX

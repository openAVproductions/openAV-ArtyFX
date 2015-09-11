
#ifndef OPENAV_ARTYFX_SATMA_UI_HXX
#define OPENAV_ARTYFX_SATMA_UI_HXX

#include "avtk.hxx"

namespace Avtk
{
  class Widget;
  class Maximizer;
};

class SatmaUI : public Avtk::UI
{
  public:
    SatmaUI(PuglNativeWindow parent = 0);
    virtual ~SatmaUI(){}
    
    virtual void widgetValueCB( Avtk::Widget* widget);
    
    virtual void lv2PortEvent(  uint32_t index,
                                uint32_t buffer_size,
                                uint32_t format, 
                                const void* buffer );

    Avtk::Maximizer* graph;
    Avtk::Dial* dial1;
    Avtk::Dial* dial2;
};


#endif // OPENAV_ARTYFX_SATMA_UI_HXX

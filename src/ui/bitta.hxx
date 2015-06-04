
#ifndef OPENAV_ARTYFX_BITTA_UI_HXX
#define OPENAV_ARTYFX_BITTA_UI_HXX

#include "../avtk/avtk.hxx"

namespace Avtk
{
  class Widget;
  class Bitcrusher; 
};

class BittaUI : public Avtk::UI
{
  public:
    BittaUI(PuglNativeWindow parent = 0);
    virtual ~BittaUI(){}
    
    virtual void widgetValueCB( Avtk::Widget* widget);
    
    virtual void lv2PortEvent(  uint32_t index,
                                uint32_t buffer_size,
                                uint32_t format, 
                                const void* buffer );

    Avtk::Bitcrusher* graph;
    Avtk::Dial* dial1;
    Avtk::Dial* dial2;
    Avtk::Dial* dial3;
};


#endif // OPENAV_ARTYFX_BITTA_UI_HXX

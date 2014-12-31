
#ifndef OPENAV_AVTK_TEST_UI_HXX
#define OPENAV_AVTK_TEST_UI_HXX

#include "avtk/avtk.hxx"

#include "header.c"

namespace Avtk
{
  class Widget;
};

class TestUI : public Avtk::UI
{
  public:
    /// Set a NativeWindow for embedding: ignore for standalone 
    TestUI(PuglNativeWindow parent = 0);
    
    /// demo function, changes all widgets to value
    void setAllWidgets( Avtk::Widget* w, float value );
    
    void widgetValueCB( Avtk::Widget* widget);
  
    Avtk::Button* groupToggler;
    Avtk::Button* momentary;
    Avtk::Button* momentaryOut;
    
    Avtk::Group* group1;
    
    Avtk::Scroll* scroll;
    
    Avtk::Slider* vertSlider;
    Avtk::Slider* horiSlider;
    
    Avtk::EventEditor* editor;
    
    Avtk::List* list;
    Avtk::List* list2;
    Avtk::Waveform* waveform;
};


#endif // OPENAV_AVTK_TEST_UI_HXX

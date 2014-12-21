
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
    
    Avtk::Group* group1;
    
    Avtk::List* list;
    Avtk::List* list2;
    Avtk::Waveform* waveform;
  
  private:
    // stores the origin widget, to avoid recusive calling
    Avtk::Widget* originSetAllWidget;
    
    
};


#endif // OPENAV_AVTK_TEST_UI_HXX

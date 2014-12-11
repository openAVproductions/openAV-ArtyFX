
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
    TestUI();
    
    /// demo function, changes all widgets to value
    void setAllWidgets( Avtk::Widget* w, float value );
  
  private:
    // stores the origin widget, to avoid recusive calling
    Avtk::Widget* originSetAllWidget;
};


#endif // OPENAV_AVTK_TEST_UI_HXX

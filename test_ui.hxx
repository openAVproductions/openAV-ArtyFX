
#ifndef OPENAV_AVTK_TEST_UI_HXX
#define OPENAV_AVTK_TEST_UI_HXX

#include "avtk/avtk.hxx"

// FIXME: refactor?
static void widgetCB(Avtk::Widget* w, void* ud)
{
  printf( "widgetCB(), label = %s\n", w->label.c_str() );
}

class TestUI : public Avtk::UI
{
  public:
    TestUI():
      Avtk::UI( 610, 430 )
    {
      Avtk::Widget* w = 0;
      
      w = new Avtk::Button( 10, 10, 75, 22, "Layer 1" );
      w->callback = widgetCB;
      w->callbackUD = this;
      add( w );
      
      w = new Avtk::Button( 10, 40, 75, 22, "Layer 2" );
      w->callback = widgetCB;
      w->callbackUD = this;
      add( w );
      
      w = new Avtk::Button( 10, 70, 75, 22, "Layer 3" );
      w->callback = widgetCB;
      w->callbackUD = this;
      add( w );
    }
};

#endif // OPENAV_AVTK_TEST_UI_HXX

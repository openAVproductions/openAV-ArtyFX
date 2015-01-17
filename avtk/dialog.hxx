
#ifndef OPENAV_AVTK_DIALOG_HXX
#define OPENAV_AVTK_DIALOG_HXX

#include "widget.hxx"

namespace Avtk
{

class Button;

class Dialog : public Widget
{
  public:
    Dialog( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
    virtual ~Dialog(){}
    
    enum BUTTONS {
      OK = 0,
      OK_CANCEL,
    };
    
    void show( const char* text, BUTTONS b = OK );
    
    
    virtual void draw( cairo_t* cr );
  
  private:
    BUTTONS buttons_;
    std::string contents;
    
    Avtk::Button* ok;
    Avtk::Button* cancel;
};

};

#endif // OPENAV_AVTK_DIALOG_HXX

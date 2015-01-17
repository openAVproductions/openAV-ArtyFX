
#ifndef OPENAV_AVTK_DIALOG_HXX
#define OPENAV_AVTK_DIALOG_HXX

#include "group.hxx"

namespace Avtk
{

class Button;

class Dialog : public Group
{
  public:
    Dialog( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
    virtual ~Dialog(){}
    
    enum BUTTONS {
      OK = 0,
      OK_CANCEL,
    };
    
    /// returns 1 on OK or YES, 0 on cancel or NO, -1 on error
    int run( const char* text, BUTTONS b = OK );
    
    // to highjack the OK / Cancel buttons events
    virtual void valueCB( Avtk::Widget* widget);
    
    virtual void draw( cairo_t* cr );
  
  private:
    BUTTONS buttons_;
    std::string contents;
    
    int returnVal;
    
    Avtk::Button* ok;
    Avtk::Button* cancel;
};

};

#endif // OPENAV_AVTK_DIALOG_HXX


#ifndef OPENAV_AVTK_EVENT_EDITOR_HXX
#define OPENAV_AVTK_EVENT_EDITOR_HXX

#include "widget.hxx"

#include <string>
#include <vector>
#include <valarray>

#include "midi.hxx"

namespace Avtk
{

class EventEditor : public Widget
{
  public:
    EventEditor( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
    
    virtual void draw( cairo_t* cr );
    
    void zoom( float z )
    {
      if( z > 0.9 )
      {
        w_ *= 2.0;
        h_ *= 2.0;
      }
      else
      {
        w_ *= 0.5;
        h_ *= 0.5;
      }
      
    }
    
    /// sets the data to be displayed in the Editor
    void setEvents( SeqEventList* events );
  
  private:
    // current view
    int startKey;
    int keyCount;
    float keyHpx;
    
    // event vector:
    SeqEventList* events;
    
};

};

#endif // OPENAV_AVTK_EVENT_EDITOR_HXX

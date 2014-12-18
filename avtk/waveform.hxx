
#ifndef OPENAV_AVTK_WAVEFORM_HXX
#define OPENAV_AVTK_WAVEFORM_HXX

#include "widget.hxx"

#include "yasper.hxx"
#include <vector>

namespace Avtk
{

class Waveform : public Widget
{
  public:
    Waveform( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
    
    virtual void draw( cairo_t* cr );
    
    void show( std::vector<float> data );
    
    /// call to change the zoom level:
    ///   1.0 is normal
    ///   2.0 shows half the sample data
    ///   4.0 shows a quater etc
    void zoom( float zoomLevel = 1.0f );
    
    /// sets the center sample for the zoom, 0 is start, 0.5 mid, 1 is end
    void zoomOffset( float percentageOffset );
    
    void setStartPoint( float percent );
  
  private:
    /// cache the drawn waveform for speed
    bool newWaveform;
    cairo_t*          waveformCr;
    cairo_surface_t*  waveformSurf;
    
    /// this ptr (when not zero) points to a vector that contains the audio data
    /// to be drawn. Its a shared ptr in order to ease memory book-keeping.
    yasper::ptr< std::vector<float> > audioData;
    
    /// view parameters
    float zoom_;
    float zoomOffset_;
    float startPoint;
};

};

#endif // OPENAV_AVTK_WAVEFORM_HXX

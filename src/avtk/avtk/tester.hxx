
#ifndef OPENAV_AVTK_TESTER_HXX
#define OPENAV_AVTK_TESTER_HXX

#ifdef AVTK_TESTER

#include "pugl/pugl.h"

#include <string>
#include <vector>

namespace Avtk
{

class UI;

class AvtkEvent
{
  public:
    AvtkEvent( const PuglEvent* e, double time ) :
      event( *e ),
      timestamp( time )
    {
    }
    
    PuglEvent event;
    double  timestamp;
};

/** Tester
 * The tester class records AVTK / PUGL events, and serializes them to JSON.
 * This allows program interaction to be recorded at the widget level, and then
 * replayed to test the program.
 */
class Tester
{
  public:
    Tester( Avtk::UI* ui );
    
    /// start a recording pass, the test will be saved as avtkTests/<testName>.json
    void record( const char* testName );
    bool recording(){return recording_;}
    
    /// records the event being passed in
    void handle( const PuglEvent* event );
    
    /// stops the recording pass, and writes the JSON file
    void recordStop();
    
    /// runs a specific JSON test file. Set @param ignoreTimestamps to true for
    /// testing timing-independant UI behaviour. 
    int runTest( const char* testName, bool ignoreTimestamps = false );
    
    void writeTest( const char* filename );
    
    /// process gets called by the UI repeatedly, when Tester is playing back,
    /// events are injected to the UI, otherwise no action is taken.
    void process();
  
  private:
    UI* ui;
    bool playing_;
    bool recording_;
    double startRecTime;
    double startPlayTime;
    
    long playEventNum;
    
    std::string name;
    std::vector<AvtkEvent> events;
    
    double getTime();
};

}; // Avtk

#endif // AVTK_TESTER

#endif // OPENAV_AVTK_TESTER_HXX

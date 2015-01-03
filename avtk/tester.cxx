

#include "tester.hxx"

#include "ui.hxx"
#include "picojson.hxx"

#include <list>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif


namespace Avtk
{

Tester::Tester( Avtk::UI* ui_ ) :
  ui( ui_ ),
  playing_( false ),
  recording_( false ),
  startRecTime( 0 ),
  startPlayTime( 0 ),
  playEventNum( 0 )
{
}

void Tester::record( const char* n )
{
  playing_ = false;
  recording_ = true;
  name = n;
  startRecTime = getTime();
  printf("%s , %lf\n", __PRETTY_FUNCTION__, startRecTime );
  events.clear();
}

void Tester::handle( const PuglEvent* event )
{
  if( !playing_ )
  {
    if( event->type != PUGL_MOTION_NOTIFY )
    {
      events.push_back( AvtkEvent( event, getTime() - startRecTime ) );
    }
  }
}

void Tester::recordStop()
{
  if( !events.size() )
  {
    printf("No events recorded.\n" );
    return;
  }
  AvtkEvent* e = &events.at(0);
  
  for(int i = 0; i < events.size(); i++ )
  {
    printf("Event %i, type %i\n", i, e->event.type );
    e++;
  }
  
  printf("%s stopping AVTK TEST %s\n", __PRETTY_FUNCTION__, name.c_str() );
  recording_ = false;
}

void Tester::process()
{
  if( playing_ )
  {
    if ( playEventNum < events.size() )
    {
      if( events.at( playEventNum ).timestamp < getTime() - startPlayTime )
      {
        ui->event( &events.at( playEventNum ).event );
        playEventNum++;
      }
    }
    else
    {
      playing_   = 0;
      recording_ = 0;
    }
  }
}

int Tester::runTest( const char* testName, bool ignoreTimestamps )
{
  playing_ = true;
  
  // get the time of the play start
  startPlayTime = getTime();
  
  if( recording_ )
    recordStop();
  
  playEventNum = 0;
  
  /*
  printf("%s running test %s : num events %i\n", __PRETTY_FUNCTION__, testName, events.size() );
  
  if( events.size() > 0 )
  {
    //AvtkEvent* e = &events.at(0);
    for(int i = 0; i < events.size(); i++ )
    {
      //printf("Event %i, type %i\n", i, events.at(0).event.type );
      
      
    }
  }
  */
  
  return 0;
  
  /*
  try
  {
    std::ifstream ifs;
    ifs.open ( testName, std::ifstream::in);
    
    picojson::value v;
    ifs >> v;
    
    if( ifs.fail() )
    {
      printf("Theme::load() %s : File doesn't exist, abort.\n", testName );
      //std::cerr << picojson::get_last_error() << std::endl;
      return -1;
    }
    
    const char* items[5] = 
    {
      "bg",
      "bg-dark",
      "fg",
      "fg-dark",
      "highlight"
    };
    
    for( int i = 0; i < 5; i++ )
    {
      // extract the 3 ints from the array, and store into Color array
      int colNum = 0;
      picojson::array list = v.get( items[i] ).get<picojson::array>();
      for (picojson::array::iterator iter = list.begin(); iter != list.end(); ++iter)
      {
        int tmp = (int)(*iter).get("c").get<double>();
        //printf("%s = %lf\r\n", items[i], tmp );
        ///colors[i].c[colNum++] = tmp;
      }
    }
  }
  catch( ... )
  {
    printf("Theme::load() Error loading theme from %s : falling back to default.Double check file-exists and JSON contents valid.\n", testName );
    // *any* error, and we don't use the theme
    return -1;
  }
  */
}

double Tester::getTime()
{
#ifdef _WIN32
  LARGE_INTEGER count;
  QueryPerformanceCounter(&count);
  double time = count.QuadPart * m_reciprocalFrequency;
#else
  struct timespec interval;
  clock_gettime(CLOCK_MONOTONIC, &interval);
  double time = interval.tv_sec + interval.tv_nsec * 0.000000001f;
#endif

  return time;
};

}; // Avtk


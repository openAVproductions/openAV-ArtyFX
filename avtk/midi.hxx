/*
 * Author: Harry van Haaren 2013
 *         harryhaaren@gmail.com
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LUPPP_MIDI_H
#define LUPPP_MIDI_H

#include <vector>

/** SeqEvent
 *  Simple bass class for deriving sequencable events from. Allows for future
 *  expansion of the Sequencer to hold arbitrary new types of events.
 */
class SeqEventBase
{
  public:
    /// create new EventBase: optionally specify time and duration
    SeqEventBase( float time = -1, float duration = -1 );
    virtual ~SeqEventBase(){};
    
    /// get the event's unique ID
    unsigned long getID();
    
    /// get/set the time of the event
    float getTime(){return time;}
    void setTime(float t){time = t;}
    
    /// returns integer beat from start of loop
    int getBeat()
    {
      return int(time);
    }
    
    float getDuration()
    {
      return duration;
    }
    
    /// returns amount of time from the previous beat as float between 0-1.
    /// 0 is exactly on the beat, while 0.25 is 25% of the way from the current
    /// beat towards the next beat
    float getOffset()
    {
      return ( time - int(time) );
    }
    
  protected:
    void setUniqueID( unsigned long uid )
    {
      ID = uid;
    }
    
  private:
    unsigned long ID;
    static unsigned long privateID;
    
    /// The timestamp of any SeqEvent.
    float time;
    
    /// If duration makes sense, the events end time is time+duration
    float duration;
};

/** MidiEvent
 *  Represents a single MIDI event. Always 3 bytes, interpret as standard MIDI.
 */
class MidiEvent : public SeqEventBase
{
  public:
    /// create empty midi event
    MidiEvent();
    
    /// creates a new MidiEvent specifying start time & duration: optional data
    MidiEvent( float time, float duration, unsigned char* d = 0);
    
    void setData( unsigned char* d );
    unsigned char data[3];
};

/** SeqEventList
 *  Holds a list of SeqEvents, allows for adding / swapping contents.
 *  MidiLooper uses instances to hold & interact with clip contents.
 */
class SeqEventList
{
  public:
    SeqEventList(int scene);
    
    int getLoopLength();
    void setLoopLenght(int l);
    
    void add( MidiEvent* m );
    void modify( MidiEvent m );
    
    int numEvents();
    SeqEventBase* getNext();
    
    /// move to next event. True if there was one, false if not
    bool moveToNextEvent();
    
    /// restart the clip from the beginning
    void queueFromStart();
    
    /// clear all contents, freeing memory: non-RT safe
    void nonRtClear();
  
  private:
    unsigned long ID;
    static unsigned long privateID;
    
    int scene;
    
    int loopLengthBeats;
    
    /// number of events in clip
    unsigned int eventCount;
    
    /// current index of event
    unsigned int eventIndex;
    
    /// holds pointers to each event
    int eventCapacity;
    std::vector<SeqEventBase*> events;
};



#endif // LUPPP_MIDI_H


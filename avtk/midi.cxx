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

#include "midi.hxx"

#include <stdio.h>
#include <cstring>

unsigned long SeqEventBase::privateID = 0;
unsigned long SeqEventList::privateID = 0;

SeqEventBase::SeqEventBase( float t, float dur )
{
  ID = privateID++;
  time = t;
  duration = dur;
}

MidiEvent::MidiEvent() :
  SeqEventBase( -1, 0 )
{
  memset( data, 0, sizeof(unsigned char) * 3);
}

MidiEvent::MidiEvent(float time, float duration, unsigned char* srcData) :
  SeqEventBase( time, duration )
{
  if ( srcData )
    setData( srcData );
  else // nuke data to zero
    memset( data, 0, sizeof(unsigned char) * 3);
}

void MidiEvent::setData(unsigned char* d)
{
  memcpy( data, d, sizeof(unsigned char) * 3 );
}


#define DEFAULT_NUM_EVENTS 1024

SeqEventList::SeqEventList(int s) :
  ID( privateID++ ),
  scene( s ),
  loopLengthBeats( 8 ),
  eventCount(0),
  eventIndex(0),
  eventCapacity( DEFAULT_NUM_EVENTS )
{
  events.reserve(DEFAULT_NUM_EVENTS);
}

SeqEventList::~SeqEventList()
{
  nonRtClear();
}

void SeqEventList::add( MidiEvent* m )
{
  float t = m->getTime();
  
  // insert the MidiEvent
  bool inserted = false;
  for(unsigned int i = 0; i < eventCount; i++)
  {
    if( t < events.at(i)->getTime() )
    {
      events.insert( events.begin() + i, (SeqEventBase*)m );
      inserted = true;
      printf("inserted\n");
      break;
    }
  }
  
  // or append to back
  if ( !inserted )
    events.push_back( m );
  
  eventCount++;
  
  /*
  
  //printf("Event list:\n");
  for(unsigned int i = 0; i < eventCount; i++)
  {
    //printf( "%f\n", events.at(i)->getTime() );
  }
  //printf("\n");
  
  */
}

void SeqEventList::nonRtClear()
{
  for(unsigned int i = 0; i < eventCount; i++)
  {
    delete events.at(i);
  }
  eventIndex = 0;
  eventCount = 0;
}

int SeqEventList::getLoopLength()
{
  return loopLengthBeats;
}

void SeqEventList::setLoopLenght(int l)
{
  loopLengthBeats = l;
}

void SeqEventList::modify( MidiEvent m )
{
  
}

int SeqEventList::numEvents()
{
  return eventCount;
}

void SeqEventList::queueFromStart()
{
  eventIndex = 0;
}

SeqEventBase* SeqEventList::getNext()
{
  if ( eventIndex < eventCount )
    return events.at( eventIndex );
  
  return 0;
}

bool SeqEventList::moveToNextEvent()
{
  eventIndex++;
  
  if( eventIndex < eventCount )
  {
    return true;
  }
  
  return false;
}




#include "eventeditor.hxx"

#include "ui.hxx"
#include "theme.hxx"

#include <stdio.h>

using namespace Avtk;

EventEditor::EventEditor( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ )
{
  //clickMode( CLICK_MOMENTARY );
  
  // piano keyboard starts at 24, has 88
  startKey = 24 + 12 * 1;
  keyCount = 88;
  
  // height of each key, based on current zoom level
  keyHpx = h_ / float(keyCount);
  
  events = new SeqEventList( 0 );
  
  // note 48 on, 96 velocity
  unsigned char ev1[] = { 0x90, 84, 0x7F };
  unsigned char ev2[] = { 0x90, 72, 0x70 };
  unsigned char ev3[] = { 0x90, 60, 0x60 };
  unsigned char ev4[] = { 0x90, 48, 0x55 };
  unsigned char ev5[] = { 0x90, 36, 0x4F };
  unsigned char ev6[] = { 0x90, 44, 0x30 };
  unsigned char ev7[] = { 0x90, 42, 0x20 };
  unsigned char ev8[] = { 0x90, 40, 0x00 };
  events->add( new MidiEvent( 0.00, 0.25, ev1 ) );
  events->add( new MidiEvent( 0.50, 0.5 , ev2 ) );
  events->add( new MidiEvent( 1.00, 0.75, ev3 ) );
  events->add( new MidiEvent( 1.50, 0.5 , ev4 ) );
  events->add( new MidiEvent( 1.50, 0.65, ev1 ) );
  events->add( new MidiEvent( 2.00, 0.15, ev5 ) );
  events->add( new MidiEvent( 2.50, 0.5 , ev6 ) );
  events->add( new MidiEvent( 3.00, 0.05, ev7 ) );
  events->add( new MidiEvent( 3.50, 0.2 , ev8 ) );
  
  events->setLoopLenght( 4 );
}

void EventEditor::draw( cairo_t* cr )
{
  cairo_save( cr );
  
  cairo_set_line_width(cr, 1.0);
  
  // fill whole background
  cairo_rectangle( cr, x_, y_, w_, h_);
  cairo_set_source_rgb( cr, 89 / 255.f,  89 / 255.f ,  89 / 255.f  );
  cairo_fill( cr );
  
  // height of each key, based on current zoom level
  keyHpx = h_ / float(keyCount);
  
  drawKeyboard( cr );
  
  // bar / beat lines
  int numBeats = events->getLoopLength();
  
  // FIXME: hardcoded beats-per-bar, get from GUI?
  for(int i = 0; i < numBeats * 4; i++ )
  {
    int xPx = x_ + w_ * i / numBeats;
    cairo_move_to( cr, xPx, y_ + 0 );
    cairo_line_to( cr, xPx, y_ + h_ );
    
    if ( i % 4 == 0 )
      cairo_set_source_rgba( cr, 35 / 255.f,  35 / 255.f ,  35 / 255.f, 0.8 );
    else
      cairo_set_source_rgba( cr, 35 / 255.f,  35 / 255.f ,  35 / 255.f, 0.4 );
    cairo_stroke( cr );
  }
  
  /// draw notes:
  events->queueFromStart();
  SeqEventBase* e = events->getNext();
  
  if ( !e )
  {
    printf("empty SeqEventList\n");
    return;
  }
  // calculate horizontal pixel count per beat for this zoom level
  int beatPx = float(w_) / events->getLoopLength();
  
  /// loop over events: highjacked increment to advance to next note
  for( ; e ; events->moveToNextEvent() , e = events->getNext() )
  {
    float startTime = e->getTime();
    float durationPx  = e->getDuration() * beatPx;
    
    MidiEvent* m = dynamic_cast<MidiEvent*>(e);
    if ( m )
    {
      float note = (m->data[1] - (startKey+4)) / float(keyCount);
      
      // check if note is in current view
      if( m->data[1] > startKey && m->data[1] < startKey + keyCount )
      {
        float velo = m->data[2] / 127.f;
        
        // draw the note rectangle
        int notePx = h_ - (note) * h_;
        cairo_rectangle( cr, x_ + beatPx * e->getTime(), y_ + notePx + 1, durationPx, keyHpx - 2);
        
        // per note colour: velocity!
        static const float max[] = {1.0, 0.28, 0.0};
        static const float mid[] = {0.9, 0.90, 0.9};
        static const float min[] = {0.0, 0.71, 1.0};
        if ( velo > 0.5 )
        {
          float v = (velo-0.5) * 2; // normalize 0-1
          cairo_set_source_rgb( cr, mid[0]+(max[0]-mid[0])*v, mid[1]+(max[1]-mid[1])*v, mid[2]+(max[2]-mid[2])*v );
        }
        else
        {
          float v = 1 - velo * 2; // normalize 0-1
          cairo_set_source_rgb( cr, mid[0]+(min[0]-mid[0])*v, mid[1]+(min[1]-mid[1])*v, mid[2]+(min[2]-mid[2])*v );
        }
        
        // note outline: depends on total pixel height of note
        if ( keyHpx > 3 )
        {
          cairo_fill_preserve( cr );
          
          cairo_set_line_width(cr, 1.0);
          cairo_set_source_rgba( cr, 0 / 255.f, 0 / 255.f , 0 / 255.f , 0.8 );
          cairo_stroke( cr );
        }
        else
        {
          cairo_fill( cr );
          cairo_set_source_rgba( cr, 0 / 255.f, 0 / 255.f , 0 / 255.f , 0.8 );
        }
        
        // draw velocity line
        float lineHeight = y_ + notePx + keyHpx/2;
        cairo_move_to( cr, x_ + 3 + beatPx * e->getTime(), lineHeight );
        cairo_line_to( cr, x_ + beatPx * e->getTime() + ((durationPx-6) * velo), lineHeight );
        cairo_set_line_width(cr, 1.3);
        cairo_stroke( cr );
      } // note is in current view
      
    } // if dynamic cast == MidiEvent*
    
  } // events
  
  cairo_restore( cr );
}

void EventEditor::drawKeyboard( cairo_t* cr )
{
  
  /*
  // draw keyboard style horizontal sections
  startKey = 24 + 12 * 1;
  keyCount = 88;
  keyHpx
  */
  
  for( int i = 0; i < keyCount; i++)
  {
    int notePx = h_ - (i) * keyHpx;
    cairo_move_to( cr, x_     , notePx );
    cairo_line_to( cr, x_ + w_, notePx );
    //cairo_rectangle( cr, x_, notePx, w_, keyHpx - 2);
  }
}


#include "eventeditor.hxx"

#include "ui.hxx"
#include "theme.hxx"

#include <stdio.h>
#include <sstream>

using namespace Avtk;

EventEditor::EventEditor( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
	Widget( ui, x_, y_, w_, h_, label_ )
{
	//clickMode( CLICK_MOMENTARY );

	// piano keyboard starts at 24, has 88
	startKey = 1;
	keyCount = 88;

	events = new SeqEventList( 0 );

	unsigned char ev[] = { 0x90, 0, 0x00 };

	for( int i = 0; i < 127; i++ ) {
		events->add( new MidiEvent( 0.250, 1 - i / 64.f, ev ) );
		ev[1] += 2;
		ev[2] = ((i % 13)/12.f) * 127;
	}

	// note 48 on, 96 velocity
	/*
	unsigned char ev1[] = { 0x90, 84, 0x7F };
	unsigned char ev2[] = { 0x90, 72, 0x70 };
	unsigned char ev3[] = { 0x90, 60, 0x60 };
	unsigned char ev4[] = { 0x90, 48, 0x55 };
	unsigned char ev5[] = { 0x90, 36, 0x4F };
	unsigned char ev6[] = { 0x90, 63, 0x30 };
	unsigned char ev7[] = { 0x90, 54, 0x20 };
	unsigned char ev8[] = { 0x90, 40, 0x00 };
	events->add( new MidiEvent( 0.00, 0.25, ev1 ) );
	events->add( new MidiEvent( 0.50, 0.5 , ev2 ) );
	events->add( new MidiEvent( 1.00, 0.75, ev3 ) );
	events->add( new MidiEvent( 1.50, 0.5 , ev4 ) );
	events->add( new MidiEvent( 1.50, 0.65, ev1 ) );
	events->add( new MidiEvent( 2.00, 0.15, ev5 ) );
	events->add( new MidiEvent( 0.50, 0.15, ev6 ) );
	events->add( new MidiEvent( 1.00, 1.00, ev7 ) );
	events->add( new MidiEvent( 2.50, 0.2 , ev8 ) );
	*/

	events->setLoopLenght( 4 );
}

EventEditor::~EventEditor()
{
	delete events;
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
	int keyHpx = h_ / keyCount;

	//int oneKeyPx = h_ / keyCount;

	drawKeyboard( cr );

	// bar / beat lines
	int numBeats = events->getLoopLength();

	// FIXME: hardcoded beats-per-bar, get from GUI?
	for(int i = 0; i < numBeats * 4; i++ ) {
		int xPx = x_ + 20 + (w_-20) * i / numBeats;
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

	if ( !e ) {
		printf("empty SeqEventList\n");
		return;
	}
	// calculate horizontal pixel count per beat for this zoom level
	int beatPx = float(w_ - 20) / events->getLoopLength();

	/// loop over events: highjacked increment to advance to next note
	for( ; e ; events->moveToNextEvent() , e = events->getNext() ) {
		float startTime = e->getTime();
		float durationPx  = e->getDuration() * beatPx;

		MidiEvent* m = dynamic_cast<MidiEvent*>(e);
		if ( m ) {
			float note = (m->data[1]) / float(keyCount);
			// check if note is in current view
			if( m->data[1] > startKey && m->data[1] < startKey + keyCount ) {
				float velo = m->data[2] / 127.f;

				// draw the note rectangle
				//int notePx = h_ - (note) * h_;
				int notePx = y_ + h_ - (note) * h_;
				cairo_rectangle( cr, x_ + 20 + beatPx * e->getTime(), y_ + notePx + 2, durationPx, keyHpx - 4);

				// per note colour: velocity!
				static const float max[] = {1.0, 0.28, 0.0};
				static const float min[] = {0.9, 0.90, 0.9};
				static const float mid[] = {0.0, 0.71, 1.0};
				if ( velo > 0.5 ) {
					float v = (velo-0.5) * 2; // normalize 0-1
					cairo_set_source_rgb( cr, mid[0]+(max[0]-mid[0])*v, mid[1]+(max[1]-mid[1])*v, mid[2]+(max[2]-mid[2])*v );
				} else {
					float v = 1 - velo * 2; // normalize 0-1
					cairo_set_source_rgb( cr, mid[0]+(min[0]-mid[0])*v, mid[1]+(min[1]-mid[1])*v, mid[2]+(min[2]-mid[2])*v );
				}

				// note outline: depends on total pixel height of note
				if ( keyHpx > 3 ) {
					cairo_fill_preserve( cr );

					cairo_set_line_width(cr, 1.0);
					cairo_set_source_rgba( cr, 0 / 255.f, 0 / 255.f , 0 / 255.f , 0.8 );
					cairo_stroke( cr );
				} else {
					cairo_fill( cr );
					cairo_set_source_rgba( cr, 0 / 255.f, 0 / 255.f , 0 / 255.f , 0.8 );
				}

				// draw velocity line
				float lineHeight = y_ + notePx + keyHpx/2;
				cairo_move_to( cr, x_ + 20 + 3 + beatPx * e->getTime(), lineHeight );
				cairo_line_to( cr, x_ + 20 + beatPx * e->getTime() + ((durationPx-6) * velo), lineHeight );
				cairo_set_line_width(cr, 1.3);
				cairo_stroke( cr );

				// MIDI note debugging (shows numbers on notes)
				cairo_set_source_rgba( cr, 0 / 255.f, 1 / 255.f , 0 / 255.f , 1 );
				std::stringstream s;
				s << int(m->data[1]);
				cairo_move_to(cr, x_ + beatPx * e->getTime(), lineHeight  );
				cairo_show_text( cr, s.str().c_str() );

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

	int oneKeyPx = h_ / (keyCount-7);

	// h_ == startKey

	int blackKeys[] = {0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0};

	// Draw label
	cairo_text_extents_t extents;
	cairo_set_font_size(cr, 14.0);
	cairo_text_extents(cr, "10", &extents);

	int notePx = y_ + h_ - 0 * oneKeyPx;

	for( int i = 0; notePx > y_ ; i++) {
		int iMod = i%12;

		if( iMod == 0 || iMod == 5 ) {
			if( iMod == 0 ) {
				cairo_set_line_width(cr, 1.3);
			} else {
				cairo_set_line_width(cr, 0.5);
			}
			// dark line between B-C and E-F
			cairo_move_to( cr, x_     , y_ + notePx );
			cairo_line_to( cr, x_ + w_, y_ + notePx  );
			cairo_set_source_rgba( cr, 0 / 255.f, 0 / 255.f , 0 / 255.f , 0.8 );
			cairo_stroke( cr );
		}

		if( blackKeys[iMod] ) {

			cairo_rectangle( cr, x_, y_ + notePx - oneKeyPx, w_, oneKeyPx);
			cairo_set_source_rgba( cr, 0 / 255.f, 0 / 255.f , 0 / 255.f , 0.2 );
			cairo_fill( cr );

			// MIDI note debugging (shows numbers on keyboard)
			cairo_set_source_rgba( cr, 1 / 255.f, 1 / 255.f , 1 / 255.f , 0.8 );
			std::stringstream s;
			s << i;
			cairo_move_to(cr, x_ + 4, notePx );
			cairo_show_text( cr, s.str().c_str() );

		} else {
			cairo_rectangle( cr, x_, y_ + notePx - oneKeyPx, 20, oneKeyPx);
			cairo_set_source_rgba( cr, 1 , 1, 1, 0.6 );
			cairo_fill( cr );
		}

		notePx = h_ - i * oneKeyPx;
	}

	// line down past keyboard
	cairo_move_to( cr, x_ + 20, y_ );
	cairo_line_to( cr, x_ + 20, y_ + h_ );
	cairo_set_line_width(cr, 1.3);
	cairo_stroke( cr );

}


#include "event_editor_test.hxx"

#include "../avtk/utils.hxx"
#include "../avtk/theme.hxx"

#include <sstream>

#define WIDTH  920
#define HEIGHT 520

EventEditorUI::EventEditorUI(PuglNativeWindow parent) :
	Avtk::UI( WIDTH, HEIGHT, parent, "AVTK : Event Editor" )
{
	themes.push_back( new Avtk::Theme( this, "default.avtk" ) );
	Widget::theme_ = themes.front();

	Avtk::Widget* w = 0;

	// Editor
	scroll = new Avtk::Scroll( this, 10, 10, WIDTH-20, HEIGHT-20, "EventEditorScroll" );
	scroll->setCtrlZoom( true );

	int scale = 4;
	editor = new Avtk::EventEditor( this, 0, 0, 240*scale, 240*scale, "EventEditor" );
	editor->value( true );
	editor->visible( true );

	scroll->set( editor );
	scroll->end();
}


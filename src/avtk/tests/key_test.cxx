
#include "key_test.hxx"

#include "../avtk/theme.hxx"

#include <stdio.h>

#define WIDTH  120
#define HEIGHT  60

KeyUI::KeyUI(PuglNativeWindow parent) :
	Avtk::UI( WIDTH, HEIGHT, parent, "AVTK : Key Test" )
{
	themes.push_back( new Avtk::Theme( this, "default.avtk" ) );
	Widget::theme_ = themes.front();
}

int KeyUI::handle( const PuglEvent* e )
{
	if( e->type == PUGL_KEY_PRESS ) {
		printf("Key %i\n", e->key.character );
	}
	//printf("KeyUI Handle called\n" );
	return 0;
}

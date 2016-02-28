
#include "list_test.hxx"

#include "../avtk/theme.hxx"

#include <sstream>

#define WIDTH  500
#define HEIGHT 200

static void cb( Avtk::Widget* w, void* ud )
{
	printf("ListTest - cb value %f\n", ((Avtk::List*)w)->value() );
}

ListUI::ListUI(PuglNativeWindow parent) :
	Avtk::UI( WIDTH, HEIGHT, parent, "AVTK : List" )
{
	themes.push_back( new Avtk::Theme( this, "default.avtk" ) );
	Widget::theme_ = themes.front();


	listH = new Avtk::List( this, 60, 10, 0, 30, "ListH" );
	listH->mode( Group::HEIGHT_EQUAL );
	listH->resizeMode( Group::RESIZE_FIT_TO_CHILDREN );

	Avtk::Widget* w = 0;
	for(int i = 0; i < 8; i++ ) {
		listH->addItem( "Tst-H" );
	}


	listV = new Avtk::List( this, 10, 10, 40, 40, "ListV" );
	listV->mode( Group::WIDTH_EQUAL );
	listV->resizeMode( Group::RESIZE_FIT_TO_CHILDREN );

	w = 0;
	for(int i = 0; i < 5; i++ ) {
		listV->addItem( "Tst-V" );
	}


	listH->callback = cb;
	listV->callback = cb;
}


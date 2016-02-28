
#include "group_test.hxx"

#include "../avtk/theme.hxx"

#include <sstream>

#define WIDTH  460
#define HEIGHT 260

GroupUI::GroupUI(PuglNativeWindow parent) :
	Avtk::UI( WIDTH, HEIGHT, parent, "AVTK : Event Editor" )
{
	themes.push_back( new Avtk::Theme( this, "default.avtk" ) );
	Widget::theme_ = themes.front();

	Avtk::Widget* w = 0;

	group1 = new Avtk::Group( this, 10, 10, 80, 200, "Group1" );
	group1->mode( Group::WIDTH_EQUAL );

	w = new Avtk::Button( this, 0, 0, 120, 20, "GrpBtn1" );
	printf("group button 1 parent : %s\n", w->parent()->label() );
	w = new Avtk::Button( this, 0, 0, 120, 20, "GrpBtn2" );
	printf("group button 2 parent : %s\n", w->parent()->label() );
	group1->end();

	w = new Avtk::Button( this, 86, 60, 120, 20, "No-Group-Btn" );
	printf("no group button parent : %s\n", w->parent()->label() );

}



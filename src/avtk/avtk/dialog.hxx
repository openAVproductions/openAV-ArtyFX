
#ifndef OPENAV_AVTK_DIALOG_HXX
#define OPENAV_AVTK_DIALOG_HXX

#include "group.hxx"

namespace Avtk
{

class Button;

class Dialog : public Group
{
public:
	Dialog( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
	virtual ~Dialog() {}

	enum BUTTONS {
		OK = 0,
		OK_CANCEL,
	};

	/// Optionally pass in X,Y co-ords of the mouse, and the OK/YES button will
	/// be positioned under the mouse cursor.
	void run( const char* header, const char* content, BUTTONS b = OK, int x = -1, int y = -1 );

	virtual void draw( cairo_t* cr );

private:
	BUTTONS buttons_;
	std::string contents;

	int mx, my;

	Avtk::Button* ok;
	Avtk::Button* cancel;

	// internally highjack the OK / Cancel buttons events
	virtual void valueCB( Avtk::Widget* widget);
};

};

#endif // OPENAV_AVTK_DIALOG_HXX

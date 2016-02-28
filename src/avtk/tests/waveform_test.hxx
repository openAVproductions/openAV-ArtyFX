
#ifndef OPENAV_AVTK_WAVEFORM_TEST_HXX
#define OPENAV_AVTK_WAVEFORM_TEST_HXX

#include "../avtk/avtk.hxx"

namespace Avtk
{
class Widget;
};

class WaveformUI : public Avtk::UI
{
public:
	/// Set a NativeWindow for embedding: ignore for standalone
	WaveformUI(PuglNativeWindow parent = 0);
	virtual ~WaveformUI() {}

	// ignore widget values, empty function body
	virtual void widgetValueCB( Avtk::Widget* widget) {}

	Avtk::Waveform* waveform;
};


#endif // OPENAV_AVTK_WAVEFORM_TEST_HXX

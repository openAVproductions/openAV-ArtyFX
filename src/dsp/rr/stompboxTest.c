
// g++ AnalogFilter.cxx StompBox.cxx Waveshaper.cxx stompboxTest.c

#include "StompBox.h"

int main()
{

	StompBox s(44100);

	float tmp[128];
	for(int i = 0; i < 128; i++)
		tmp[i] = i;

	s.process( 128, tmp, tmp );

	return 0;
}

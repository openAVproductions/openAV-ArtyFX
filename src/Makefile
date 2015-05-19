
# Build the AVTK library
# It outputs a .a static lib
avtk:
	cd avtk/build && cmake .. && make -j2

CFLAGS=-g -fPIC -shared -Wl,-z,nodelete -Wl,--no-undefined

roomy:
	cd dsp && g++ $(CFLAGS) roomy.cxx

all: avtk roomy
	echo "Linking DSP .so"
	#gcc -shared -fPIC

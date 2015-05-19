
# Build the AVTK library
# It outputs a .a static lib
avtk:
	cd avtk/build && cmake .. && make -j2

CFLAGS=-g -fPIC -shared -Wl,-z,nodelete -Wl,--no-undefined

dsp:
	cd dsp && g++ $(CFLAGS) eq/*.cc rr/*.cxx ../artyfx.cxx *.cxx -lsndfile -lpthread -o artyfx.so

all: avtk roomy
	echo "Linking DSP .so"
	#gcc -shared -fPIC

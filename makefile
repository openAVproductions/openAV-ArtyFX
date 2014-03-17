
# run make in each plugins subdirectory
.PHONY:
	all

all:
	cd roomy && make
	cd masha && make
	cd ducka && make
	cd filta && make
	cd della && make
	cd satma && make
	cd kuiza && make

clean:
	cd roomy && make clean
	cd masha && make clean
	cd ducka && make clean
	cd filta && make clean
	cd della && make clean
	cd satma && make clean
	cd kuiza && make clean

install:
	cd roomy && make install
	cd masha && make install
	cd ducka && make install
	cd filta && make install
	cd della && make install
	cd satma && make install
	cd kuiza && make install

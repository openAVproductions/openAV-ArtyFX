
# run make in each plugins subdirectory
.PHONY:
	all

all:
	cd roomy && make
	cd masha && make
	cd ducka && make
	cd filta && make

install:
	cd roomy && make install
	cd masha && make install
	cd ducka && make install
	cd filta && make install

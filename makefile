
# run make in each plugins subdirectory
.PHONY:
	all

all:
	cd roomy && make
	cd masha && make
	cd ducka && make
	cd filta && make

install:
	cd roomy && make INSTALL_PATH=${LV2_PATH} install
	cd masha && make INSTALL_PATH=${LV2_PATH} install
	cd ducka && make INSTALL_PATH=${LV2_PATH} install
	cd filta && make INSTALL_PATH=${LV2_PATH} install

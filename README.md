ArtyFX OpenAV Productions
=========================

Official: [http://openavproductions.com/artyfx](http://openavproductions.com/artyfx)

Manual: [ArtyFX Manual](https://github.com/harryhaaren/openAV-ArtyFX/blob/master/manual.md)

This is the repository of an LV2 plugin suite called Arty FX.

![ArtyFX screenshot](https://raw.github.com/harryhaaren/openAV-ArtyFX/master/screenshots/artyfx_1.2.png "ArtyFX 1.2 screenshot")


Dependencies
------------
These plugins use NTK for thier interfaces. NTK can be obtained using
git clone git://git.tuxfamily.org/gitroot/non/fltk.git ntk

You'll need the LV2 headers to compile these plugins, package name:
lv2

Building
--------
Once deps are satisfied, building and installing is easy with CMake:
```
mkdir build
cd build
cmake ..
make -j4
make install
```

Testing
-------
After the INSTALL step Ardour3, QTractor, and any other LV2 host should
automatically find all plugins, and be able to use them. Test availablility
with 
```
$ lv2ls
```

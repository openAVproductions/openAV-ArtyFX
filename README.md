ArtyFX OpenAV Productions
=========================

Official: [http://openavproductions.com/artyfx](http://openavproductions.com/artyfx)

Manual: [ArtyFX Manual](https://github.com/harryhaaren/openAV-ArtyFX/blob/master/manual.md)

This is the repository of an LV2 plugin suite called Arty FX.

![ArtyFX screenshot](https://raw.github.com/harryhaaren/openAV-ArtyFX/master/screenshots/artyfx_1.3.png "ArtyFX 1.3 screenshot")


Dependencies
------------
ArtyFX uses AVTK for its user interfaces, and therefore
depends only on Cairo and X. The LV2 headers are needed
to build the plugins.

On Debian Jessie (current stable) the following will grab
the dependencies needed to build ArtyFX:
```
sudo apt-get install libcairo-dev lv2-dev lv2core
```

Building
--------

The Meson build system is now preferred over the old CMake based system,
as configuring builds in Meson is easier. The following steps build ArtyFX
using Meson:

```
meson build_dir
cd build_dir
ninja
ninja install
```

Legacy builds
~~~~~~~~~~~~~
Previous versions of ArtyFX were built using CMake, which is still working
but will be deprecated in future. Steps below for the old method:
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
$ lv2ls | grep openav
```

Contact
-------
Harry van Haaren   <harryhaaren@gmail.com>
OpenAV Productions 
http://www.openavproductions.com


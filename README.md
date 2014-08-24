ArtyFX OpenAV Productions
=========================

Official: [http://openavproductions.com/artyfx](http://openavproductions.com/artyfx)

Manual: [ArtyFX Manual](https://github.com/harryhaaren/openAV-ArtyFX/master/manual.md)

This is the repository of an LV2 plugin suite called Arty FX.

![ArtyFX screenshot](https://raw.github.com/harryhaaren/openAV-ArtyFX/master/screenshots/artyfx_1.1.png "ArtyFX 1.1 screenshot")


Dependencies
------------
These plugins use NTK for thier interfaces. NTK can be obtained using
git clone git://git.tuxfamily.org/gitroot/non/fltk.git ntk

Most other deps will already be installed on most systems:
cairo
cairomm-1.0
lv2


Building
--------
Once deps are satisfied, building and installing into ~/.lv2/ is easy,
just run make as usual:

make
make install


Testing
-------
After the INSTALL step Ardour3, QTractor, and any other LV2 host should
automatically find all plugins, and be able to use them. Test availablility
with 
```
$ lv2ls
```

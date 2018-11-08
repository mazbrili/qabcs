qABCs is copyright by DanSoft (dik@inbox.ru), 2018

This program is licensed under the terms and conditions of
the GNU General Public License (GPLv3+).
Please read the 'COPYING' file for more information.

qABCs is an educational tool/game targeted at children.
It is designed to aid in learning alphabet through visual
and audio samples. As a side function it can also help to
familiarize one with a keyboard.

To build qABCs you need Qt5 and qmake.
Full list of dependencies for linux: see pkg/qabcs.spec.

Building:
qmake
make

Also you can use Qt Creator for building.
Just open qabcs.pro and build qABCs (`Ctrl+B`).

Installation (under root):
make install

Testing without installation:
qmake QMAKE_CXXFLAGS+='-DGLOBAL_PATH_USERDATA=\\\"..\\\"'
make
./Bin/qabcs

Translations:
GUI and desktop file: https://www.transifex.com/Magic/qabcs
ABC: create your own abs.json (more detailed format) or
abs.properties (easier, but not detailed format). Use pictures
from abcs/all/pics.

![screenshot1 images](https://bitbucket.org/admsasha/qabcs/raw/master/screenshots/screenshot1.png)
![screenshot2 images](https://bitbucket.org/admsasha/qabcs/raw/master/screenshots/screenshot2.png)
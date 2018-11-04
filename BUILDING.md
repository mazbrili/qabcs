How to build qABCs?
==================

qABCs can be built for Microsoft Windows, GNU/Linux. Generally it uses Qt Library (http://qt-project.org).
You must use Qt 5.

General pre-requirements
-----------------------
* GCC >= 5.5 (Windows: mingw-gcc)
* Make (Windows: mingw-make)
* Qt 5


Qt configuration in Linux
-------------------------
qABCs uses following modules: QtGui, QtWidgets, QtMultimedia. To build qABCs you should to install corresponding "-devel" packages. To compile translations qABCs uses lupdate and lrelease tools. Therefore it may require additional packages, for example, 'qttools5'.

Building in Windows
-------------------
* Download sources from https://bitbucket.org/admsasha/qabcs/get/master.zip
* Unpack master.zip to current folder
* `cd admsasha-qabcs-*`
* To run Qt tools it needs Qt 'bin' directories to be in PATH. For example:
* `set PATH=C:\Qt\5.9.4\mingw55_32\bin;C:\Qt/Tools/mingw55_32\bin;%PATH%`
* `qmake qabcs.pro`
* `make release`

Building in Linux
-----------------
* Download sources from https://bitbucket.org/admsasha/qabcs/get/master.zip
* Unpack master.zip to current folder
* `cd admsasha-qabcs-*`
* `qmake` (or `qmake qabcs.pro`)
* `make`

Installation
------------
`make install`
or
`make install INSTALL_ROOT=dir`
if you want to install in the specific directory (dir).

Using Qt Creator
----------------
Also you can use Qt Creator. Just open qabcs.pro and build qABCs (`Ctrl+B`).


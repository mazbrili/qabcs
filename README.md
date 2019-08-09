# ENGLISH #

qABCs is copyright by DanSoft (dik@inbox.ru), 2019

This program is licensed under the terms and conditions of
the GNU General Public License (GPLv3+); either version
3 of the License, or (at your option) any later version.
Please read the 'COPYING' file for more information.

qABCs is an educational tool/game targeted at children.
It is designed to aid in learning alphabet through visual
and audio samples. As a side function it can also help to
familiarize one with a keyboard.

To build qABCs you need Qt5 and qmake.
Full list of dependencies for linux: see pkg/qabcs.spec.

### Building: ###
~~~~
qmake
make
~~~~
Also you can use Qt Creator for building.
Just open qabcs.pro and build qABCs (`Ctrl+B`).

### Installation (under root): ###
~~~~
make install
~~~~

### Testing without installation: ###
~~~~
qmake QMAKE_CXXFLAGS+='-DGLOBAL_PATH_USERDATA=\\\"..\\\"'
make
./Bin/qabcs
~~~~
### Translations: ###
ABC, GUI and desktop file: https://www.transifex.com/Magic/qabcs

### Current languages: ###
GUI translated into many popular languages.
ABCs translated on American English (US), Russian, United
Kingdom English (UK), Ukrainian, German.

### Packages and build specifications: ###
Repository of your distribution is the first.
Arch, CentOS, Debian, Fedora, Mageia, Suse, Ubuntu:
https://build.opensuse.org/package/show/home:admsasha/qABCs

Applications for Windows and sources:
https://bitbucket.org/admsasha/qabcs/downloads

![screenshot1 images](https://bitbucket.org/admsasha/qabcs/raw/master/screenshots/screenshot1.png)

***

# RUSSIAN/РУССКИЙ #
Авторское право на qABCs принадлежит DanSoft (dik@inbox.ru),
2019

Эта программа лицензирована в соответствии с положениями GNU
General Public License (GPLv3+); либо версии 3 Лицензии, либо
(по вашему выбору) любой более поздней версии. Пожалуйста,
прочитайте файл 'COPYING' для получения дополнительной
информации.

qABCs - это образовательная утилита/игра, ориентированная на
детей. Она призвана помочь в изучении алфавита с помощью
визуальных и звуковых образцов. В качестве вспомогательной
функции она также может помочь познакомиться с клавиатурой.

Для сборки qABCs вам нужны Qt5 и qmake.
Полный список зависимостей для linux: см. pkg/qabcs.spec.

### Сборка: ###
~~~~
qmake
make
~~~~
Также вы можете использовать Qt Creator для сборки.
Просто откройте qabcs.pro и соберите qABCs (`Ctrl+B`).

### Установка (под root): ###
~~~~
make install
~~~~

### Тестирование без установки: ###
~~~~
qmake QMAKE_CXXFLAGS+='-DGLOBAL_PATH_USERDATA=\\\"..\\\"'
make
./Bin/qabcs
~~~~
### Переводы: ###
Алфавит, ГУИ и desktop файл: https://www.transifex.com/Magic/qabcs

### Текущие языки: ###
ГУИ переведён на многие популярные языки.
Алфавиты переведены на американский английский (US), русский,
английский Соединённого Королевства (UK), украинский, немецкий.

### Пакеты  и спецификации для сборки: ###
Репозиторий вашего дистрибутива первичен.
Arch, CentOS, Debian, Fedora, Mageia, Suse, Ubuntu:
https://build.opensuse.org/package/show/home:admsasha/qABCs

Приложения для Windows и сурсы:
https://bitbucket.org/admsasha/qabcs/downloads

![screenshot2 images](https://bitbucket.org/admsasha/qabcs/raw/master/screenshots/screenshot2.png)

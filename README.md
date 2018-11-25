# ENGLISH #

qABCs is copyright by DanSoft (dik@inbox.ru), 2018

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
GUI and desktop file: https://www.transifex.com/Magic/qabcs
ABC: create your own abs.json (more detailed format) or
abs.properties (easier, but not detailed format). Use pictures
from abcs/all/pics. Use noises from abcs/all/noises. Use
template scripts to generate sound files from text to speech or
record voice.

### Current languages: ###
GUI translated on English, Russian, Dutch, Ukrainian, Estonian,
Portuguese, Bulgarian, Malay.
ABCs translated on American English (US), Russian, United
Kingdom English (UK), Ukrainian.

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
2018

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
ГУИ и desktop файл: https://www.transifex.com/Magic/qabcs
Алфавит: создайте ваш собственный abs.json (более детальный
формат) или abs.properties (проще, но не детальный формат).
Используйте картинки из abcs/all/pics. Используйте шумы из
abcs/all/noises. Используйте шаблонные скрипты для генерации
звуковых файлов из текста в речь или запишите голос.

### Текущие языки: ###
ГУИ переведён на английский, русский, голландский, украинский,
эстонский, португальский, болгарский, малайский.
Алфавиты переведены на американский английский (US), русский,
английский Соединённого Королевства (UK), украинский.

### Пакеты  и спецификации для сборки: ###
Репозиторий вашего дистрибутива первичен.
Arch, CentOS, Debian, Fedora, Mageia, Suse, Ubuntu:
https://build.opensuse.org/package/show/home:admsasha/qABCs

Приложения для Windows и сурсы:
https://bitbucket.org/admsasha/qabcs/downloads

![screenshot2 images](https://bitbucket.org/admsasha/qabcs/raw/master/screenshots/screenshot2.png)

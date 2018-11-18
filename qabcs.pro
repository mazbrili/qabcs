######################################################################
# Automatically generated by qmake (3.0) ?? ??? 3 15:18:05 2014
######################################################################

TEMPLATE = app
TARGET = qabcs
INCLUDEPATH += .

QT += widgets multimedia

ARCH = $$QMAKE_HOST.arch

CONFIG(debug, release|debug){
    MOC_DIR = .build/$$ARCH/debug
    OBJECTS_DIR = .build/$$ARCH/debug
    UI_DIR = .build/$$ARCH/debug
    RCC_DIR = .build/$$ARCH/debug
}
CONFIG(release, release|debug){
    MOC_DIR = .build/$$ARCH/release
    OBJECTS_DIR = .build/$$ARCH/release
    UI_DIR = .build/$$ARCH/release
    RCC_DIR = .build/$$ARCH/release
}
QMAKE_LINK_OBJECT_SCRIPT = .build/$$ARCH/object_script


#RESOURCES += qabcs.qrc
win32:RC_FILE = qabcs.rc

DESTDIR = Bin

gcc {
    QMAKE_CXXFLAGS += -std=gnu++11
    QMAKE_CXXFLAGS += -pedantic -pedantic-errors
    QMAKE_CXXFLAGS += -Wall -Wextra -Wformat -Wformat-security -Wno-unused-variable -Wno-unused-parameter
}

exists( langs/qabcs_en.ts ) {
      TRANSLATIONS = $$files(langs/qabcs_*.ts)
} else {
      TRANSLATIONS = langs/qabcs_en.ts \
         $$files(langs/qabcs_*.ts)
}

isEmpty(QMAKE_LRELEASE) {
    win32|os2:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
    unix {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease-qt5 }
    } else {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    }
}

!win32 {
  system($${QMAKE_LRELEASE} -silent $${_PRO_FILE_} 2> /dev/null)
}
win32 {
  system($${QMAKE_LRELEASE} $${_PRO_FILE_})
}

updateqm.input = TRANSLATIONS
updateqm.output = langs/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$QMAKE_LRELEASE -silent ${QMAKE_FILE_IN} -qm langs/${QMAKE_FILE_BASE}.qm
updateqm.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += updateqm

data_bin.path = /usr/bin/
data_bin.files = Bin/qabcs
INSTALLS += data_bin

data_app.path = /usr/share/applications/
data_app.files = pkg/qabcs.desktop
INSTALLS += data_app

data_man.path = /usr/share/man/man1/
data_man.files = pkg/qabcs.1
INSTALLS += data_man

data_pixmaps.path = /usr/share/pixmaps/
data_pixmaps.extra = ln -fs /usr/share/qabcs/images/logo/icon.xpm ${INSTALL_ROOT}/usr/share/pixmaps/qabcs.xpm
data_pixmaps.uninstall = rm -f -r ${INSTALL_ROOT}/usr/share/pixmaps/qabcs.xpm
INSTALLS += data_pixmaps

data_abcs.path = /usr/share/qabcs/abcs/
data_abcs.files = abcs/*
INSTALLS += data_abcs

data_sounds.path = /usr/share/qabcs/sounds/
data_sounds.files = sounds/*
INSTALLS += data_sounds

data_images.path = /usr/share/qabcs/images/
data_images.files = images/*
INSTALLS += data_images

data_langs.path = /usr/share/qabcs/langs/
data_langs.files = langs/*.qm langs/langs.json
INSTALLS += data_langs

# Input
SOURCES += main.cpp \
    FormAbout.cpp \
    mainwindow.cpp \
    FormHelp.cpp \
    Collection.cpp \
    FormSelectLanguage.cpp \
    SoundEngine.cpp \
    LoaderAbcFormats.cpp

FORMS += \
    FormAbout.ui \
    FormHelp.ui \
    FormSelectLanguage.ui

HEADERS += \
    FormAbout.h \
    mainwindow.h \
    FormHelp.h \
    Collection.h \
    FormSelectLanguage.h \
    config_qabcs.h \
    SoundEngine.h \
    LoaderAbcFormats.h

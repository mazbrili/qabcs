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


DESTDIR = Bin

#RESOURCES += qabcs.qrc
win32:RC_FILE = qabcs.rc


gcc {
    QMAKE_CXXFLAGS += -std=gnu++11
    QMAKE_CXXFLAGS += -pedantic -pedantic-errors
    QMAKE_CXXFLAGS += -Wall -Wextra -Wformat -Wformat-security -Wno-unused-variable -Wno-unused-parameter
}

TRANSLATIONS = qabcs_en.ts \
    qabcs_ru.ts

# Input
SOURCES += main.cpp \
    FormAbout.cpp \
    mainwindow.cpp \
    FormHelp.cpp \
    Collection.cpp \
    FormSelectLanguage.cpp

FORMS += \
    FormAbout.ui \
    mainwindow.ui \
    FormHelp.ui \
    FormSelectLanguage.ui

HEADERS += \
    FormAbout.h \
    mainwindow.h \
    FormHelp.h \
    Collection.h \
    FormSelectLanguage.h \
    config_qabcs.h

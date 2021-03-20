#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T13:09:43
#
#-------------------------------------------------

QT += network xml
QT -= gui

MODULE = ZSTrace

VERSION = 4.6.3

TEMPLATE = lib

windows {
    CONFIG += skip_target_version_ext
    DEFINES += _WINDOWS
    PLATFORM = x64
}
linux {
    DEFINES += __linux__
    PLATFORM = Linux
}

CONFIG(release, release|debug) {
    TARGET = $$MODULE
    MOC_DIR = ../../../Tmp/Moc/Libs/$$MODULE/$$PLATFORM/Release
    OBJECTS_DIR = ../../../Tmp/Int/Libs/$$MODULE/$$PLATFORM/Release
}
CONFIG(debug, release|debug) {
    TARGET = $$MODULE"d"
    MOC_DIR = ../../../Tmp/Moc/Libs/$$MODULE/$$PLATFORM/Debug
    OBJECTS_DIR = ../../../Tmp/Int/Libs/$$MODULE/$$PLATFORM/Debug
}
DESTDIR = ../../../Lib/$$PLATFORM

CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
}
DEFINES += _ZSTRACE
DEFINES += ZSTRACEDLL_EXPORTS

INCLUDEPATH += ../../../Include/Libs

LIBS += -L../../../Lib/$$PLATFORM

CONFIG(release, release|debug) {
    LIBS += -lZSSys
    LIBS += -lZSIpc
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
    LIBS += -lZSIpcd
}

SOURCES += \
    ../../../Source/Libs/ZSTrace/ZSTrcServer.cpp \
    ../../../Source/Libs/ZSTrace/ZSTrcDllMain.cpp \
    ../../../Source/Libs/ZSTrace/ZSTrcDllIf.cpp \
    ../../../Source/Libs/ZSTrace/ZSTrcClient.cpp

HEADERS += \
    ../../../Include/Libs/ZSTrace/ZSTrcServer.h \
    ../../../Include/Libs/ZSTrace/ZSTrcDllMain.h \
    ../../../Include/Libs/ZSTrace/ZSTrcDllIf.h \
    ../../../Include/Libs/ZSTrace/ZSTrcClient.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

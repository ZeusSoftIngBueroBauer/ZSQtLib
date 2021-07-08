#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T13:09:43
#
#-------------------------------------------------

QT += network xml
QT -= gui

MODULE = ZSIpcTrace

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
DEFINES += ZSIPCTRACEDLL_EXPORTS

INCLUDEPATH += ../../../Include/Libs

LIBS += -L../../../Lib/$$PLATFORM
linux {
    LIBS += -L/usr/local/lib
}

CONFIG(release, release|debug) {
    LIBS += -lZSSys
    LIBS += -lZSIpc
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
    LIBS += -lZSIpcd
}

SOURCES += \
    ../../../Source/Libs/ZSIpcTrace/ZSIpcTrcClient.cpp \
    ../../../Source/Libs/ZSIpcTrace/ZSIpcTrcDllIf.cpp \
    ../../../Source/Libs/ZSIpcTrace/ZSIpcTrcDllIfServerThread.cpp \
    ../../../Source/Libs/ZSIpcTrace/ZSIpcTrcDllMain.cpp \
    ../../../Source/Libs/ZSIpcTrace/ZSIpcTrcServer.cpp

HEADERS += \
    ../../../Include/Libs/ZSIpcTrace/ZSIpcTrcClient.h \
    ../../../Include/Libs/ZSIpcTrace/ZSIpcTrcDllIf.h \
    ../../../Include/Libs/ZSIpcTrace/ZSIpcTrcDllIfServerThread.h \
    ../../../Include/Libs/ZSIpcTrace/ZSIpcTrcDllMain.h \
    ../../../Include/Libs/ZSIpcTrace/ZSIpcTrcServer.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

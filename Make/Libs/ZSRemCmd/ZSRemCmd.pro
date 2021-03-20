#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T13:12:48
#
#-------------------------------------------------

QT += network xml
QT -= gui

MODULE = ZSRemCmd

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
DEFINES += ZSREMCMDDLL_EXPORTS

INCLUDEPATH += ../../../Include/Libs

LIBS += -L../../../Lib/$$PLATFORM

CONFIG(release, release|debug) {
    LIBS += -lZSSys
    LIBS += -lZSIpc
    LIBS += -lZSTrace
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
    LIBS += -lZSIpcd
    LIBS += -lZSTraced
}

SOURCES += \
    ../../../Source/Libs/ZSRemCmd/ZSRemCmdSubSysNode.cpp \
    ../../../Source/Libs/ZSRemCmd/ZSRemCmdsSyst.cpp \
    ../../../Source/Libs/ZSRemCmd/ZSRemCmdServer.cpp \
    ../../../Source/Libs/ZSRemCmd/ZSRemCmdParser.cpp \
    ../../../Source/Libs/ZSRemCmd/ZSRemCmdObjPool.cpp \
    ../../../Source/Libs/ZSRemCmd/ZSRemCmdObj.cpp \
    ../../../Source/Libs/ZSRemCmd/ZSRemCmdMsg.cpp \
    ../../../Source/Libs/ZSRemCmd/ZSRemCmdErrNumbers.cpp \
    ../../../Source/Libs/ZSRemCmd/ZSRemCmdDllMain.cpp \
    ../../../Source/Libs/ZSRemCmd/ZSRemCmdClient.cpp

HEADERS += \
    ../../../Include/Libs/ZSRemCmd/ZSRemCmdSubSysNode.h \
    ../../../Include/Libs/ZSRemCmd/ZSRemCmdsSyst.h \
    ../../../Include/Libs/ZSRemCmd/ZSRemCmdServer.h \
    ../../../Include/Libs/ZSRemCmd/ZSRemCmdParser.h \
    ../../../Include/Libs/ZSRemCmd/ZSRemCmdObjPool.h \
    ../../../Include/Libs/ZSRemCmd/ZSRemCmdObj.h \
    ../../../Include/Libs/ZSRemCmd/ZSRemCmdMsg.h \
    ../../../Include/Libs/ZSRemCmd/ZSRemCmdErrNumbers.h \
    ../../../Include/Libs/ZSRemCmd/ZSRemCmdDllMain.h \
    ../../../Include/Libs/ZSRemCmd/ZSRemCmdClient.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

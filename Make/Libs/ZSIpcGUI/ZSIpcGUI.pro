#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T13:08:20
#
#-------------------------------------------------

QT += network xml

MODULE = ZSIpcGUI

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
DEFINES += ZSIPCGUIDLL_EXPORTS

INCLUDEPATH += ../../../Include/Libs

LIBS += -L../../../Lib/$$PLATFORM

CONFIG(release, release|debug) {
    LIBS += -lZSSys
    LIBS += -lZSIpc
    LIBS += -lZSSysGUI
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
    LIBS += -lZSIpcd
    LIBS += -lZSSysGUId
}

SOURCES += \
    ../../../Source/Libs/ZSIpcGUI/ZSIpcTrcMsgLogWdgt.cpp \
    ../../../Source/Libs/ZSIpcGUI/ZSIpcServerWdgt.cpp \
    ../../../Source/Libs/ZSIpcGUI/ZSIpcServerDlg.cpp \
    ../../../Source/Libs/ZSIpcGUI/ZSIpcModelSrvCltConnection.cpp \
    ../../../Source/Libs/ZSIpcGUI/ZSIpcGUIDllMain.cpp \
    ../../../Source/Libs/ZSIpcGUI/ZSIpcClientWdgt.cpp \
    ../../../Source/Libs/ZSIpcGUI/ZSIpcClientDlg.cpp

HEADERS += \
    ../../../Include/Libs/ZSIpcGUI/ZSIpcTrcMsgLogWdgt.h \
    ../../../Include/Libs/ZSIpcGUI/ZSIpcServerWdgt.h \
    ../../../Include/Libs/ZSIpcGUI/ZSIpcServerDlg.h \
    ../../../Include/Libs/ZSIpcGUI/ZSIpcModelSrvCltConnection.h \
    ../../../Include/Libs/ZSIpcGUI/ZSIpcGUIDllMain.h \
    ../../../Include/Libs/ZSIpcGUI/ZSIpcClientWdgt.h \
    ../../../Include/Libs/ZSIpcGUI/ZSIpcClientDlg.h

RESOURCES = ../../../Images/Libs/ZSIpc/ZSIpc.qrc

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

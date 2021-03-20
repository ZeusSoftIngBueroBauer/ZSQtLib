#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T13:10:56
#
#-------------------------------------------------

QT += network xml

MODULE = ZSTraceGUI

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
DEFINES += ZSTRACEGUIDLL_EXPORTS

INCLUDEPATH += ../../../Include/Libs

LIBS += -L../../../Lib/$$PLATFORM

CONFIG(release, release|debug) {
    LIBS += -lZSSys
    LIBS += -lZSIpc
    LIBS += -lZSTrace
    LIBS += -lZSSysGUI
    LIBS += -lZSIpcGUI
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
    LIBS += -lZSIpcd
    LIBS += -lZSTraced
    LIBS += -lZSSysGUId
    LIBS += -lZSIpcGUId
}

SOURCES += \
    ../../../Source/Libs/ZSTraceGUI/ZSTrcSettingsWdgt.cpp \
    ../../../Source/Libs/ZSTraceGUI/ZSTrcServerDlg.cpp \
    ../../../Source/Libs/ZSTraceGUI/ZSTrcMthWdgt.cpp \
    ../../../Source/Libs/ZSTraceGUI/ZSTrcMthModel.cpp \
    ../../../Source/Libs/ZSTraceGUI/ZSTrcMsgWdgt.cpp \
    ../../../Source/Libs/ZSTraceGUI/ZSTrcMsgModel.cpp \
    ../../../Source/Libs/ZSTraceGUI/ZSTrcGUIDllMain.cpp \
    ../../../Source/Libs/ZSTraceGUI/ZSTrcCtxWdgt.cpp \
    ../../../Source/Libs/ZSTraceGUI/ZSTrcCtxModel.cpp \
    ../../../Source/Libs/ZSTraceGUI/ZSTrcClientDlg.cpp \
    ../../../Source/Libs/ZSTraceGUI/ZSTrcAdminObjPoolWdgtNameSpaces.cpp \
    ../../../Source/Libs/ZSTraceGUI/ZSTrcAdminObjPoolWdgtNameSpaceContent.cpp \
    ../../../Source/Libs/ZSTraceGUI/ZSTrcAdminObjPoolWdgt.cpp \
    ../../../Source/Libs/ZSTraceGUI/ZSTrcAdminObjPoolModel.cpp \
    ../../../Source/Libs/ZSTraceGUI/ZSTrcAdminObjPoolDlg.cpp

HEADERS += \
    ../../../Include/Libs/ZSTraceGUI/ZSTrcSettingsWdgt.h \
    ../../../Include/Libs/ZSTraceGUI/ZSTrcServerDlg.h \
    ../../../Include/Libs/ZSTraceGUI/ZSTrcMthWdgt.h \
    ../../../Include/Libs/ZSTraceGUI/ZSTrcMthModel.h \
    ../../../Include/Libs/ZSTraceGUI/ZSTrcMsgWdgt.h \
    ../../../Include/Libs/ZSTraceGUI/ZSTrcMsgModel.h \
    ../../../Include/Libs/ZSTraceGUI/ZSTrcGUIDllMain.h \
    ../../../Include/Libs/ZSTraceGUI/ZSTrcCtxWdgt.h \
    ../../../Include/Libs/ZSTraceGUI/ZSTrcCtxModel.h \
    ../../../Include/Libs/ZSTraceGUI/ZSTrcClientDlg.h \
    ../../../Include/Libs/ZSTraceGUI/ZSTrcAdminObjPoolWdgtNameSpaces.h \
    ../../../Include/Libs/ZSTraceGUI/ZSTrcAdminObjPoolWdgtNameSpaceContent.h \
    ../../../Include/Libs/ZSTraceGUI/ZSTrcAdminObjPoolWdgt.h \
    ../../../Include/Libs/ZSTraceGUI/ZSTrcAdminObjPoolModel.h \
    ../../../Include/Libs/ZSTraceGUI/ZSTrcAdminObjPoolDlg.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

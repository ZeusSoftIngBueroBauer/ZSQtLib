#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T13:10:56
#
#-------------------------------------------------

QT += network xml

MODULE = ZSIpcTraceGUI

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
DEFINES += ZSIPCTRACEGUIDLL_EXPORTS

INCLUDEPATH += ../../../Include/Libs

LIBS += -L../../../Lib/$$PLATFORM

CONFIG(release, release|debug) {
    LIBS += -lZSSys
    LIBS += -lZSIpc
    LIBS += -lZSIpcTrace
    LIBS += -lZSSysGUI
    LIBS += -lZSIpcGUI
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
    LIBS += -lZSIpcd
    LIBS += -lZSIpcTraced
    LIBS += -lZSSysGUId
    LIBS += -lZSIpcGUId
}

SOURCES += \
    ../../../Source/Libs/ZSIpcTraceGUI/ZSIpcTrcClientDlg.cpp \
    ../../../Source/Libs/ZSIpcTraceGUI/ZSIpcTrcGUIDllMain.cpp \
    ../../../Source/Libs/ZSIpcTraceGUI/ZSIpcTrcMthWdgt.cpp \
    ../../../Source/Libs/ZSIpcTraceGUI/ZSIpcTrcServerDlg.cpp \
    ../../../Source/Libs/ZSIpcTraceGUI/ZSIpcTrcSettingsWdgt.cpp

HEADERS += \
    ../../../Include/Libs/ZSIpcTraceGUI/ZSIpcTrcClientDlg.h \
    ../../../Include/Libs/ZSIpcTraceGUI/ZSIpcTrcGUIDllMain.h \
    ../../../Include/Libs/ZSIpcTraceGUI/ZSIpcTrcMthWdgt.h \
    ../../../Include/Libs/ZSIpcTraceGUI/ZSIpcTrcServerDlg.h \
    ../../../Include/Libs/ZSIpcTraceGUI/ZSIpcTrcSettingsWdgt.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

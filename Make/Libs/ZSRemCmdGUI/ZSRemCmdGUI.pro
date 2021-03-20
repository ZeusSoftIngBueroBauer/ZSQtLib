#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T13:13:12
#
#-------------------------------------------------

QT += network xml

MODULE = ZSRemCmdGUI

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
DEFINES += ZSREMCMDGUIDLL_EXPORTS

INCLUDEPATH += ../../../Include/Libs

LIBS += -L../../../Lib/$$PLATFORM

CONFIG(release, release|debug) {
    LIBS += -lZSSys
    LIBS += -lZSIpc
    LIBS += -lZSTrace
    LIBS += -lZSRemCmd
    LIBS += -lZSSysGUI
    LIBS += -lZSIpcGUI
    LIBS += -lZSTraceGUI
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
    LIBS += -lZSIpcd
    LIBS += -lZSTraced
    LIBS += -lZSRemCmdd
    LIBS += -lZSSysGUId
    LIBS += -lZSIpcGUId
    LIBS += -lZSTraceGUId
}

SOURCES += \
    ../../../Source/Libs/ZSRemCmdGUI/ZSRemCmdsDlg.cpp \
    ../../../Source/Libs/ZSRemCmdGUI/ZSRemCmdObjPoolWdgt.cpp \
    ../../../Source/Libs/ZSRemCmdGUI/ZSRemCmdObjPoolModel.cpp \
    ../../../Source/Libs/ZSRemCmdGUI/ZSRemCmdGUIDllMain.cpp

HEADERS += \
    ../../../Include/Libs/ZSRemCmdGUI/ZSRemCmdsDlg.h \
    ../../../Include/Libs/ZSRemCmdGUI/ZSRemCmdObjPoolWdgt.h \
    ../../../Include/Libs/ZSRemCmdGUI/ZSRemCmdObjPoolModel.h \
    ../../../Include/Libs/ZSRemCmdGUI/ZSRemCmdGUIDllMain.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

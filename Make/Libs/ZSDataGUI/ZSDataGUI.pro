#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T13:08:20
#
#-------------------------------------------------

QT += network xml sql

MODULE = ZSDataGUI

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
DEFINES += ZSDATAGUIDLL_EXPORTS

INCLUDEPATH += ../../../Include/Libs

LIBS += -L../../../Lib/$$PLATFORM

CONFIG(release, release|debug) {
    LIBS += -lZSSys
    LIBS += -lZSIpc
    LIBS += -lZSTrace
    LIBS += -lZSSysGUI
    LIBS += -lZSIpcGUI
    LIBS += -lZSTraceGUI
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
    LIBS += -lZSIpcd
    LIBS += -lZSTraced
    LIBS += -lZSSysGUId
    LIBS += -lZSIpcGUId
    LIBS += -lZSTraceGUId
}

SOURCES += \
    ../../../Source/Libs/ZSDataGUI/ZSDatabaseClientConnectionDlg.cpp \
    ../../../Source/Libs/ZSDataGUI/ZSDatabaseClientConnectionWdgt.cpp \
    ../../../Source/Libs/ZSDataGUI/ZSDatabaseClientTrcMsgLogWdgt.cpp \
    ../../../Source/Libs/ZSDataGUI/ZSDatabaseConnectionsDlg.cpp \
    ../../../Source/Libs/ZSDataGUI/ZSDatabaseConnectionsModel.cpp \
    ../../../Source/Libs/ZSDataGUI/ZSDatabaseConnectionsWdgt.cpp \
    ../../../Source/Libs/ZSDataGUI/ZSDatabaseTrcMsgLogWdgt.cpp \
    ../../../Source/Libs/ZSDataGUI/ZSDataGUIAux.cpp \
    ../../../Source/Libs/ZSDataGUI/ZSDataGUICommon.cpp \
    ../../../Source/Libs/ZSDataGUI/ZSDataGUIDllMain.cpp \
    ../../../Source/Libs/ZSDataGUI/ZSDataSetModel.cpp \
    ../../../Source/Libs/ZSDataGUI/ZSDataSetWdgt.cpp

HEADERS += \
    ../../../Include/Libs/ZSDataGUI/ZSDatabaseClientConnectionDlg.h \
    ../../../Include/Libs/ZSDataGUI/ZSDatabaseClientConnectionWdgt.h \
    ../../../Include/Libs/ZSDataGUI/ZSDatabaseClientTrcMsgLogWdgt.h \
    ../../../Include/Libs/ZSDataGUI/ZSDatabaseConnectionsDlg.h \
    ../../../Include/Libs/ZSDataGUI/ZSDatabaseConnectionsModel.h \
    ../../../Include/Libs/ZSDataGUI/ZSDatabaseConnectionsWdgt.h \
    ../../../Include/Libs/ZSDataGUI/ZSDatabaseTrcMsgLogWdgt.h \
    ../../../Include/Libs/ZSDataGUI/ZSDataGUIAux.h \
    ../../../Include/Libs/ZSDataGUI/ZSDataGUICommon.h \
    ../../../Include/Libs/ZSDataGUI/ZSDataGUIDllMain.h \
    ../../../Include/Libs/ZSDataGUI/ZSDataSetModel.h \
    ../../../Include/Libs/ZSDataGUI/ZSDataSetWdgt.h

RESOURCES = ../../../Images/Libs/ZSData/ZSData.qrc

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

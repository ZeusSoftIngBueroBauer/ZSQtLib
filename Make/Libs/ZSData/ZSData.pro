#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T13:06:39
#
#-------------------------------------------------

QT += network xml sql
QT -= gui

MODULE = ZSData

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
DEFINES += ZSDATADLL_EXPORTS

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
    ../../../Source/Libs/ZSData/ZSDataAux.cpp \
    ../../../Source/Libs/ZSData/ZSDatabase.cpp \
    ../../../Source/Libs/ZSData/ZSDatabaseClient.cpp \
    ../../../Source/Libs/ZSData/ZSDatabaseClientGateway.cpp \
    ../../../Source/Libs/ZSData/ZSDatabaseClientGatewayThread.cpp \
    ../../../Source/Libs/ZSData/ZSDatabaseGateway.cpp \
    ../../../Source/Libs/ZSData/ZSDatabaseGatewayThread.cpp \
    ../../../Source/Libs/ZSData/ZSDatabaseGatewayZSXML.cpp \
    ../../../Source/Libs/ZSData/ZSDataCommon.cpp \
    ../../../Source/Libs/ZSData/ZSDataDllMain.cpp \
    ../../../Source/Libs/ZSData/ZSDataExceptions.cpp \
    ../../../Source/Libs/ZSData/ZSDataMsg.cpp \
    ../../../Source/Libs/ZSData/ZSDataProtocol.cpp \
    ../../../Source/Libs/ZSData/ZSDataSet.cpp \
    ../../../Source/Libs/ZSData/ZSDataSetObject.cpp \
    ../../../Source/Libs/ZSData/ZSDataSetObjGroup.cpp \
    ../../../Source/Libs/ZSData/ZSDataTable.cpp

HEADERS += \
    ../../../Include/Libs/ZSData/ZSDataAux.h \
    ../../../Include/Libs/ZSData/ZSDatabase.h \
    ../../../Include/Libs/ZSData/ZSDatabaseClient.h \
    ../../../Include/Libs/ZSData/ZSDatabaseClientGateway.h \
    ../../../Include/Libs/ZSData/ZSDatabaseClientGatewayThread.h \
    ../../../Include/Libs/ZSData/ZSDatabaseGateway.h \
    ../../../Include/Libs/ZSData/ZSDatabaseGatewayThread.h \
    ../../../Include/Libs/ZSData/ZSDatabaseGatewayZSXML.h \
    ../../../Include/Libs/ZSData/ZSDataCommon.h \
    ../../../Include/Libs/ZSData/ZSDataDllMain.h \
    ../../../Include/Libs/ZSData/ZSDataExceptions.h \
    ../../../Include/Libs/ZSData/ZSDataMsg.h \
    ../../../Include/Libs/ZSData/ZSDataProtocol.h \
    ../../../Include/Libs/ZSData/ZSDataSet.h \
    ../../../Include/Libs/ZSData/ZSDataSetObject.h \
    ../../../Include/Libs/ZSData/ZSDataSetObjGroup.h \
    ../../../Include/Libs/ZSData/ZSDataTable.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

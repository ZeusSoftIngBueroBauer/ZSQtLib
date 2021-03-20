#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T13:08:20
#
#-------------------------------------------------

QT += network xml

MODULE = ZSDiagram

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
DEFINES += ZSDIAGRAMDLL_EXPORTS

INCLUDEPATH += ../../../Include/Libs

LIBS += -L../../../Lib/$$PLATFORM

CONFIG(release, release|debug) {
    LIBS += -lZSSys
    LIBS += -lZSPhysVal
    LIBS += -lZSIpc
    LIBS += -lZSTrace
    LIBS += -lZSSysGUI
    LIBS += -lZSPhysValGUI
    LIBS += -lZSIpcGUI
    LIBS += -lZSTraceGUI
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
    LIBS += -lZSPhysVald
    LIBS += -lZSIpcd
    LIBS += -lZSTraced
    LIBS += -lZSSysGUId
    LIBS += -lZSPhysValGUId
    LIBS += -lZSIpcGUId
    LIBS += -lZSTraceGUId
}

SOURCES += \
    ../../../Source/Libs/ZSDiagram/ZSDiagObj.cpp \
    ../../../Source/Libs/ZSDiagram/ZSDiagObjAxisLabel.cpp \
    ../../../Source/Libs/ZSDiagram/ZSDiagObjCurve.cpp \
    ../../../Source/Libs/ZSDiagram/ZSDiagObjErrInfo.cpp \
    ../../../Source/Libs/ZSDiagram/ZSDiagObjGrid.cpp \
    ../../../Source/Libs/ZSDiagram/ZSDiagObjHistogram.cpp \
    ../../../Source/Libs/ZSDiagram/ZSDiagObjLabel.cpp \
    ../../../Source/Libs/ZSDiagram/ZSDiagObjLine.cpp \
    ../../../Source/Libs/ZSDiagram/ZSDiagObjMarker.cpp \
    ../../../Source/Libs/ZSDiagram/ZSDiagObjProgressBar.cpp \
    ../../../Source/Libs/ZSDiagram/ZSDiagObjTable.cpp \
    ../../../Source/Libs/ZSDiagram/ZSDiagObjValueProvider.cpp \
    ../../../Source/Libs/ZSDiagram/ZSDiagram.cpp \
    ../../../Source/Libs/ZSDiagram/ZSDiagramAux.cpp \
    ../../../Source/Libs/ZSDiagram/ZSDiagramDllMain.cpp \
    ../../../Source/Libs/ZSDiagram/ZSDiagramFrameStyles.cpp \
    ../../../Source/Libs/ZSDiagram/ZSDiagramImageStyles.cpp \
    ../../../Source/Libs/ZSDiagram/ZSDiagramLabelStyles.cpp \
    ../../../Source/Libs/ZSDiagram/ZSDiagramLineStyles.cpp \
    ../../../Source/Libs/ZSDiagram/ZSDiagramMsg.cpp \
    ../../../Source/Libs/ZSDiagram/ZSDiagramToolTipStyles.cpp \
    ../../../Source/Libs/ZSDiagram/ZSDiagScale.cpp \
    ../../../Source/Libs/ZSDiagram/ZSDiagTrace.cpp

HEADERS += \
    ../../../Include/Libs/ZSDiagram/ZSDiagObj.h \
    ../../../Include/Libs/ZSDiagram/ZSDiagObjAxisLabel.h \
    ../../../Include/Libs/ZSDiagram/ZSDiagObjCurve.h \
    ../../../Include/Libs/ZSDiagram/ZSDiagObjErrInfo.h \
    ../../../Include/Libs/ZSDiagram/ZSDiagObjGrid.h \
    ../../../Include/Libs/ZSDiagram/ZSDiagObjHistogram.h \
    ../../../Include/Libs/ZSDiagram/ZSDiagObjLabel.h \
    ../../../Include/Libs/ZSDiagram/ZSDiagObjLine.h \
    ../../../Include/Libs/ZSDiagram/ZSDiagObjMarker.h \
    ../../../Include/Libs/ZSDiagram/ZSDiagObjProgressBar.h \
    ../../../Include/Libs/ZSDiagram/ZSDiagObjTable.h \
    ../../../Include/Libs/ZSDiagram/ZSDiagObjValueProvider.h \
    ../../../Include/Libs/ZSDiagram/ZSDiagram.h \
    ../../../Include/Libs/ZSDiagram/ZSDiagramAux.h \
    ../../../Include/Libs/ZSDiagram/ZSDiagramDllMain.h \
    ../../../Include/Libs/ZSDiagram/ZSDiagramFrameStyles.h \
    ../../../Include/Libs/ZSDiagram/ZSDiagramImageStyles.h \
    ../../../Include/Libs/ZSDiagram/ZSDiagramLabelStyles.h \
    ../../../Include/Libs/ZSDiagram/ZSDiagramLineStyles.h \
    ../../../Include/Libs/ZSDiagram/ZSDiagramMsg.h \
    ../../../Include/Libs/ZSDiagram/ZSDiagramToolTipStyles.h \
    ../../../Include/Libs/ZSDiagram/ZSDiagScale.h \
    ../../../Include/Libs/ZSDiagram/ZSDiagTrace.h

RESOURCES = ../../../Images/Libs/ZSDiagram/ZSDiagram.qrc

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

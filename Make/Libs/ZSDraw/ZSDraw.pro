#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T13:08:20
#
#-------------------------------------------------

QT += network xml

MODULE = ZSDraw

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
DEFINES += ZSDRAWDLL_EXPORTS

INCLUDEPATH += ../../../Include/Libs

LIBS += -L../../../Lib/$$PLATFORM

CONFIG(release, release|debug) {
    LIBS += -lZSSys
    LIBS += -lZSPhysVal
    LIBS += -lZSPhysSizesGeometry
    LIBS += -lZSPhysSizesKinematics
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
    LIBS += -lZSPhysSizesGeometryd
    LIBS += -lZSPhysSizesKinematicsd
    LIBS += -lZSIpcd
    LIBS += -lZSTraced
    LIBS += -lZSSysGUId
    LIBS += -lZSPhysValGUId
    LIBS += -lZSIpcGUId
    LIBS += -lZSTraceGUId
}

SOURCES += \
    ../../../Source/Libs/ZSDraw/ZSDrawDlgFormatGraphObjs.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawDlgPageSetup.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawDllMain.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawGraphicsItemsModel.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawGraphObj.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawGraphObjConnectionLine.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawGraphObjConnectionPoint.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawGraphObjEllipse.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawGraphObjGroup.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawGraphObjImage.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawGraphObjLabel.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawGraphObjLine.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawGraphObjModelEntry.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawGraphObjPoint.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawGraphObjPolygon.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawGraphObjPolyline.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawGraphObjRect.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawGraphObjSelectionPoint.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawGraphObjText.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawingScene.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawingView.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawObjFactoriesModel.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawObjFactory.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawObjFactoryConnectionLine.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawObjFactoryConnectionPoint.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawObjFactoryEllipse.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawObjFactoryGroup.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawObjFactoryImage.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawObjFactoryLine.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawObjFactoryPoint.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawObjFactoryPolygon.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawObjFactoryPolyline.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawObjFactoryRect.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawObjFactoryText.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawPageSetup.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawSettings.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjs.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsFillStyle.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsGeometry.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsLineStyle.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsObjName.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsText.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsTextStyle.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawWdgtPageSetup.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawWdgtPageSetupDrawingPaper.cpp \
    ../../../Source/Libs/ZSDraw/ZSDrawWdgtPageSetupDrawingScene.cpp

HEADERS += \
    ../../../Include/Libs/ZSDraw/ZSDrawDlgFormatGraphObjs.h \
    ../../../Include/Libs/ZSDraw/ZSDrawDlgPageSetup.h \
    ../../../Include/Libs/ZSDraw/ZSDrawDllMain.h \
    ../../../Include/Libs/ZSDraw/ZSDrawGraphicsItemsModel.h \
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObj.h \
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjConnectionLine.h \
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjConnectionPoint.h \
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjEllipse.h \
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjGroup.h \
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjImage.h \
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjLabel.h \
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjLine.h \
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjModelEntry.h \
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjPoint.h \
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjPolygon.h \
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjPolyline.h \
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjRect.h \
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjSelectionPoint.h \
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjText.h \
    ../../../Include/Libs/ZSDraw/ZSDrawingScene.h \
    ../../../Include/Libs/ZSDraw/ZSDrawingView.h \
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoriesModel.h \
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoriesPool.h \
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactory.h \
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoryConnectionLine.h \
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoryConnectionPoint.h \
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoryEllipse.h \
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoryGroup.h \
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoryImage.h \
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoryLine.h \
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoryPoint.h \
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoryPolygon.h \
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoryPolyline.h \
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoryRect.h \
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoryText.h \
    ../../../Include/Libs/ZSDraw/ZSDrawPageSetup.h \
    ../../../Include/Libs/ZSDraw/ZSDrawSettings.h \
    ../../../Include/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjs.h \
    ../../../Include/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsFillStyle.h \
    ../../../Include/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsGeometry.h \
    ../../../Include/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsLineStyle.h \
    ../../../Include/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsObjName.h \
    ../../../Include/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsText.h \
    ../../../Include/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsTextStyle.h \
    ../../../Include/Libs/ZSDraw/ZSDrawWdgtPageSetup.h \
    ../../../Include/Libs/ZSDraw/ZSDrawWdgtPageSetupDrawingPaper.h \
    ../../../Include/Libs/ZSDraw/ZSDrawWdgtPageSetupDrawingScene.h

RESOURCES = ../../../Images/Libs/ZSDraw/ZSDraw.qrc

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

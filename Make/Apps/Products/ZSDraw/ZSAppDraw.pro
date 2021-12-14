#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T15:57:36
#
#-------------------------------------------------

QT += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

MODULE = ZSAppDraw

VERSION = 4.6.3

TEMPLATE = app

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
    MOC_DIR = ../../../../Tmp/Moc/Apps/Products/$$MODULE/$$PLATFORM/Release
    OBJECTS_DIR = ../../../../Tmp/Int/Apps/Products/$$MODULE/$$PLATFORM/Release
}
CONFIG(debug, release|debug) {
    TARGET = $$MODULE"d"
    MOC_DIR = ../../../../Tmp/Moc/Apps/Products/$$MODULE/$$PLATFORM/Debug
    OBJECTS_DIR = ../../../../Tmp/Int/Apps/Products/$$MODULE/$$PLATFORM/Debug
}
DESTDIR = ../../../../Bin/$$PLATFORM

CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
}
DEFINES += _ZSTRACE

INCLUDEPATH += ../../../../Include/Apps/Products/ZSDraw
INCLUDEPATH += ../../../../Include/Libs
linux {
    INCLUDEPATH += /usr/local/include
}

LIBS += -L../../../../Lib/$$PLATFORM
linux {
    LIBS += -L/usr/local/lib
}

CONFIG(release, release|debug) {
    LIBS += -lZSSys
    LIBS += -lZSTest
    LIBS += -lZSPhysVal
    LIBS += -lZSPhysSizesGeometry
    LIBS += -lZSIpc
    LIBS += -lZSTrace
    LIBS += -lZSSysGUI
    LIBS += -lZSTestGUI
    LIBS += -lZSIpcGUI
    LIBS += -lZSTraceGUI
    LIBS += -lZSDraw
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
    LIBS += -lZSTestd
    LIBS += -lZSPhysVald
    LIBS += -lZSPhysSizesGeometryd
    LIBS += -lZSIpcd
    LIBS += -lZSTraced
    LIBS += -lZSSysGUId
    LIBS += -lZSTestGUId
    LIBS += -lZSIpcGUId
    LIBS += -lZSTraceGUId
    LIBS += -lZSDrawd
}

SOURCES += \
    ../../../../Source/Apps/Products/ZSDraw/App.cpp \
    ../../../../Source/Apps/Products/ZSDraw/Main.cpp \
    ../../../../Source/Apps/Products/ZSDraw/MainWindow.cpp \
    ../../../../Source/Apps/Products/ZSDraw/Test.cpp \
    ../../../../Source/Apps/Products/ZSDraw/TestStepElectricity.cpp \
    ../../../../Source/Apps/Products/ZSDraw/TestStepElectricityCapacitors.cpp \
    ../../../../Source/Apps/Products/ZSDraw/TestStepElectricityInductors.cpp \
    ../../../../Source/Apps/Products/ZSDraw/TestStepElectricityResistors.cpp \
    ../../../../Source/Apps/Products/ZSDraw/TestStepElectricitySwitches.cpp \
    ../../../../Source/Apps/Products/ZSDraw/TestStepElectricityTransistors.cpp \
    ../../../../Source/Apps/Products/ZSDraw/TestStepStandardShapes.cpp \
    ../../../../Source/Apps/Products/ZSDraw/TestStepStandardShapesConnections.cpp \
    ../../../../Source/Apps/Products/ZSDraw/TestStepStandardShapesEllipses.cpp \
    ../../../../Source/Apps/Products/ZSDraw/TestStepStandardShapesGroups.cpp \
    ../../../../Source/Apps/Products/ZSDraw/TestStepStandardShapesImages.cpp \
    ../../../../Source/Apps/Products/ZSDraw/TestStepStandardShapesLines.cpp \
    ../../../../Source/Apps/Products/ZSDraw/TestStepStandardShapesPoints.cpp \
    ../../../../Source/Apps/Products/ZSDraw/TestStepStandardShapesPolygons.cpp \
    ../../../../Source/Apps/Products/ZSDraw/TestStepStandardShapesPolylines.cpp \
    ../../../../Source/Apps/Products/ZSDraw/TestStepStandardShapesRectangles.cpp \
    ../../../../Source/Apps/Products/ZSDraw/TestStepStandardShapesTexts.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjElectricity.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityCapacitor.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityDiode.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityInductor.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityResistor.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjElectricitySwitch.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityTransistor.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityVoltageSource.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjWdgt.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtCheckBox.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtComboBox.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtGroupBox.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtLabel.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtLineEdit.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtPushButton.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityCapacitor.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityDiode.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityInductor.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityResistor.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricitySwitch.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityTransistor.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityVoltageSource.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtCheckBox.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtComboBox.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtGroupBox.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtLabel.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtLineEdit.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtPushButton.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityCapacitor.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityDiode.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityInductor.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityResistor.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricitySwitch.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityTransistor.cpp \
    ../../../../Source/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityVoltageSource.cpp

HEADERS += \
    ../../../../Include/Apps/Products/ZSDraw/App.h \
    ../../../../Include/Apps/Products/ZSDraw/MainWindow.h \
    ../../../../Include/Apps/Products/ZSDraw/Test.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjElectricity.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityCapacitor.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityDiode.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityInductor.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityResistor.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjElectricitySwitch.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityTransistor.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityVoltageSource.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjWdgt.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtCheckBox.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtComboBox.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtGroupBox.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtLabel.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtLineEdit.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtPushButton.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityCapacitor.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityDiode.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityInductor.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityResistor.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricitySwitch.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityTransistor.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityVoltageSource.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtCheckBox.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtComboBox.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtGroupBox.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtLabel.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtLineEdit.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtPushButton.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityCapacitor.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityDiode.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityInductor.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityResistor.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricitySwitch.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityTransistor.h \
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityVoltageSource.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/bin
    } else {
        target.path = /usr/bin
    }
    INSTALLS += target
}

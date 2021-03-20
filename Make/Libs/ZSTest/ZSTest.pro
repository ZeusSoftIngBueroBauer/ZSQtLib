#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T13:06:39
#
#-------------------------------------------------

QT += network xml
QT -= gui

MODULE = ZSTest

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
DEFINES += ZSTESTDLL_EXPORTS

INCLUDEPATH += ../../../Include/Libs

LIBS += -L../../../Lib/$$PLATFORM

CONFIG(release, release|debug) {
    LIBS += -lZSSys
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
}

SOURCES += \
    ../../../Source/Libs/ZSTest/ZSTest.cpp \
    ../../../Source/Libs/ZSTest/ZSTestDllMain.cpp \
    ../../../Source/Libs/ZSTest/ZSTestStep.cpp \
    ../../../Source/Libs/ZSTest/ZSTestStepGroup.cpp \
    ../../../Source/Libs/ZSTest/ZSTestStepAdminObj.cpp \
    ../../../Source/Libs/ZSTest/ZSTestStepAdminObjPool.cpp

HEADERS += \
    ../../../Include/Libs/ZSTest/ZSTest.h \
    ../../../Include/Libs/ZSTest/ZSTestDllMain.h \
    ../../../Include/Libs/ZSTest/ZSTestStep.h \
    ../../../Include/Libs/ZSTest/ZSTestStepGroup.h \
    ../../../Include/Libs/ZSTest/ZSTestStepAdminObj.h \
    ../../../Include/Libs/ZSTest/ZSTestStepAdminObjPool.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

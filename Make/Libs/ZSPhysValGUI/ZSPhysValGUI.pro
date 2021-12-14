#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T13:08:20
#
#-------------------------------------------------

QT += network xml

MODULE = ZSPhysValGUI

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
DEFINES += ZSPHYSVALGUIDLL_EXPORTS

INCLUDEPATH += ../../../Include/Libs

LIBS += -L../../../Lib/$$PLATFORM

CONFIG(release, release|debug) {
    LIBS += -lZSSys
    LIBS += -lZSPhysVal
    LIBS += -lZSSysGUI
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
    LIBS += -lZSPhysVald
    LIBS += -lZSSysGUId
}

SOURCES += \
    ../../../Source/Libs/ZSPhysValGUI/ZSPhysValGUIDllMain.cpp \
    ../../../Source/Libs/ZSPhysValGUI/ZSPhysValWdgtEditPhysVal.cpp \
    ../../../Source/Libs/ZSPhysValGUI/ZSPhysValDlgEditPhysVal.cpp

HEADERS += \
    ../../../Include/Libs/ZSPhysValGUI/ZSPhysValGUIDllMain.h \
    ../../../Include/Libs/ZSPhysValGUI/ZSPhysValWdgtEditPhysVal.h \
    ../../../Include/Libs/ZSPhysValGUI/ZSPhysValDlgEditPhysVal.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

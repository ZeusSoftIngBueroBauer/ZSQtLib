#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T15:57:36
#
#-------------------------------------------------

QT += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

MODULE = ZSAppTestIpcTraceDllIfQtApp

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
    MOC_DIR = ../../../../Tmp/Moc/Apps/Test/$$MODULE/$$PLATFORM/Release
    OBJECTS_DIR = ../../../../Tmp/Int/Apps/Test/$$MODULE/$$PLATFORM/Release
}
CONFIG(debug, release|debug) {
    TARGET = $$MODULE"d"
    MOC_DIR = ../../../../Tmp/Moc/Apps/Test/$$MODULE/$$PLATFORM/Debug
    OBJECTS_DIR = ../../../../Tmp/Int/Apps/Test/$$MODULE/$$PLATFORM/Debug
}
DESTDIR = ../../../../Bin/$$PLATFORM

CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
}
DEFINES += _ZSTRACE

INCLUDEPATH += ../../../../Include/Apps/Test/ZSIpcTraceDllIfQtApp
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
    LIBS += -lZSSysGUI
    LIBS += -lZSTestGUI
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
    LIBS += -lZSTestd
    LIBS += -lZSSysGUId
    LIBS += -lZSTestGUId
}

SOURCES += \
    ../../../../Source/Apps/Test/ZSIpcTraceDllIfQtApp/App.cpp \
    ../../../../Source/Apps/Test/ZSIpcTraceDllIfQtApp/Main.cpp \
    ../../../../Source/Apps/Test/ZSIpcTraceDllIfQtApp/MainWindow.cpp \
    ../../../../Source/Apps/Test/ZSIpcTraceDllIfQtApp/MsgTest.cpp \
    ../../../../Source/Apps/Test/ZSIpcTraceDllIfQtApp/Test.cpp \
    ../../../../Source/Apps/Test/ZSIpcTraceDllIfQtApp/TestModule1.cpp \
    ../../../../Source/Apps/Test/ZSIpcTraceDllIfQtApp/TestModule2.cpp \
    ../../../../Source/Apps/Test/ZSIpcTraceDllIfQtApp/WidgetCentral.cpp \
    ../../../../Source/Libs/ZSIpcTrace/ZSIpcTrcDllIf.cpp

HEADERS  += \
    ../../../../Include/Apps/Test/ZSIpcTraceDllIfQtApp/App.h \
    ../../../../Include/Apps/Test/ZSIpcTraceDllIfQtApp/MainWindow.h \
    ../../../../Include/Apps/Test/ZSIpcTraceDllIfQtApp/MsgTest.h \
    ../../../../Include/Apps/Test/ZSIpcTraceDllIfQtApp/Test.h \
    ../../../../Include/Apps/Test/ZSIpcTraceDllIfQtApp/TestModule1.h \
    ../../../../Include/Apps/Test/ZSIpcTraceDllIfQtApp/TestModule2.h \
    ../../../../Include/Apps/Test/ZSIpcTraceDllIfQtApp/WidgetCentral.h \
    ../../../../Include/Libs/ZSIpcTrace/ZSIpcTrcDllIf.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/bin
    } else {
        target.path = /usr/bin
    }
    INSTALLS += target
}

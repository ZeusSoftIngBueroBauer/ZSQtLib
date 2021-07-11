#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T15:57:36
#
#-------------------------------------------------

QT += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

MODULE = ZSAppTestTemplate

VERSION = 4.6.3

TEMPLATE = app

windows {
    CONFIG += skip_target_version_ext
    DEFINES += _WINDOWS
    COMPILERLIBINFIX = msvc2019
    PLATFORM = x64
}
win32-msvc2013 {
    message("win32-msvc2013") # not executed !!!
}
win32-msvc2015 {
    message("win32-msvc2015") # not executed !!!
}
win32-msvc2017 {
    message("win32-msvc2017") # not executed !!!
}
win32-msvc2019 {
    message("win32-msvc2019") # not executed !!!
}
linux {
    DEFINES += __linux__
    COMPILERLIBINFIX = "linux" # should become gcc whatever
    PLATFORM = x64
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
DESTDIR = ../../../../Bin//$$COMPILERLIBINFIX"_"$$PLATFORM

CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
}
DEFINES += _ZSTRACE

INCLUDEPATH += ../../../../Include/Apps/Test/ZSTemplate
INCLUDEPATH += ../../../../Include/Libs
linux {
    INCLUDEPATH += /usr/local/include
}

LIBS += -L../../../../Lib/$$COMPILERLIBINFIX"_"$$PLATFORM
linux {
    LIBS += -L/usr/local/lib
}

CONFIG(release, release|debug) {
    LIBS += -lZSSys"Qt"$$QT_MAJOR_VERSION"_"$$COMPILERLIBINFIX"_"$$PLATFORM
    LIBS += -lZSTest"Qt"$$QT_MAJOR_VERSION"_"$$COMPILERLIBINFIX"_"$$PLATFORM
    LIBS += -lZSSysGUI"Qt"$$QT_MAJOR_VERSION"_"$$COMPILERLIBINFIX"_"$$PLATFORM
    LIBS += -lZSTestGUI"Qt"$$QT_MAJOR_VERSION"_"$$COMPILERLIBINFIX"_"$$PLATFORM
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSys"Qt"$$QT_MAJOR_VERSION"_"$$COMPILERLIBINFIX"_"$$PLATFORM"_d"
    LIBS += -lZSTest"Qt"$$QT_MAJOR_VERSION"_"$$COMPILERLIBINFIX"_"$$PLATFORM"_d"
    LIBS += -lZSSysGUI"Qt"$$QT_MAJOR_VERSION"_"$$COMPILERLIBINFIX"_"$$PLATFORM"_d"
    LIBS += -lZSTestGUI"Qt"$$QT_MAJOR_VERSION"_"$$COMPILERLIBINFIX"_"$$PLATFORM"_d"
}

SOURCES += \
    ../../../../Source/Apps/Test/ZSTemplate/App.cpp \
    ../../../../Source/Apps/Test/ZSTemplate/Main.cpp \
    ../../../../Source/Apps/Test/ZSTemplate/MainWindow.cpp \
    ../../../../Source/Apps/Test/ZSTemplate/Test.cpp \
    ../../../../Source/Apps/Test/ZSTemplate/TSGrpTemplate.cpp \
    ../../../../Source/Apps/Test/ZSTemplate/WidgetCentral.cpp

HEADERS  += \
    ../../../../Include/Apps/Test/ZSTemplate/App.h \
    ../../../../Include/Apps/Test/ZSTemplate/MainWindow.h \
    ../../../../Include/Apps/Test/ZSTemplate/Test.h \
    ../../../../Include/Apps/Test/ZSTemplate/TSGrpTemplate.h \
    ../../../../Include/Apps/Test/ZSTemplate/WidgetCentral.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/bin
    } else {
        target.path = /usr/bin
    }
    INSTALLS += target
}

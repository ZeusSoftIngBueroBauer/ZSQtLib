#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T13:09:43
#
#-------------------------------------------------

QT += network xml
QT -= gui

MODULE = ZSIpcTrace

VERSION = 4.6.3

TEMPLATE = lib

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
    TARGET = $$MODULE"Qt"$$QT_MAJOR_VERSION"_"$$COMPILERLIBINFIX"_"$$PLATFORM
    MOC_DIR = ../../../Tmp/Moc/Libs/$$MODULE/Release_$$COMPILERLIBINFIX/$$PLATFORM
    OBJECTS_DIR = ../../../Tmp/Int/Libs/$$MODULE/Release_$$COMPILERLIBINFIX/$$PLATFORM
}
CONFIG(debug, release|debug) {
    TARGET = $$MODULE"Qt"$$QT_MAJOR_VERSION"_"$$COMPILERLIBINFIX"_"$$PLATFORM"_d"
    MOC_DIR = ../../../Tmp/Moc/Libs/$$MODULE/Debug_$$COMPILERLIBINFIX/$$PLATFORM
    OBJECTS_DIR = ../../../Tmp/Int/Libs/$$MODULE/Debug_$$COMPILERLIBINFIX/$$PLATFORM
}
DESTDIR = ../../../Lib/$$COMPILERLIBINFIX"_"$$PLATFORM

CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
}
DEFINES += _ZSTRACE
DEFINES += ZSIPCTRACEDLL_EXPORTS

INCLUDEPATH += ../../../Include/Libs

LIBS += -L../../../Lib/$$COMPILERLIBINFIX"_"$$PLATFORM
#linux {
#    LIBS += -L/usr/local/lib
#}

CONFIG(release, release|debug) {
    LIBS += -lZSSys"Qt"$$QT_MAJOR_VERSION"_"$$COMPILERLIBINFIX"_"$$PLATFORM
    LIBS += -lZSIpc"Qt"$$QT_MAJOR_VERSION"_"$$COMPILERLIBINFIX"_"$$PLATFORM
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSys"Qt"$$QT_MAJOR_VERSION"_"$$COMPILERLIBINFIX"_"$$PLATFORM"_d"
    LIBS += -lZSIpc"Qt"$$QT_MAJOR_VERSION"_"$$COMPILERLIBINFIX"_"$$PLATFORM"_d"
}

SOURCES += \
    ../../../Source/Libs/ZSIpcTrace/ZSIpcTrcClient.cpp \
    ../../../Source/Libs/ZSIpcTrace/ZSIpcTrcDllIf.cpp \
    ../../../Source/Libs/ZSIpcTrace/ZSIpcTrcDllIfServerThread.cpp \
    ../../../Source/Libs/ZSIpcTrace/ZSIpcTrcDllMain.cpp \
    ../../../Source/Libs/ZSIpcTrace/ZSIpcTrcServer.cpp

HEADERS += \
    ../../../Include/Libs/ZSIpcTrace/ZSIpcTrcClient.h \
    ../../../Include/Libs/ZSIpcTrace/ZSIpcTrcDllIf.h \
    ../../../Include/Libs/ZSIpcTrace/ZSIpcTrcDllIfServerThread.h \
    ../../../Include/Libs/ZSIpcTrace/ZSIpcTrcDllMain.h \
    ../../../Include/Libs/ZSIpcTrace/ZSIpcTrcServer.h

windows {
    SOURCEDIR = ..\..\..\Lib\\$$COMPILERLIBINFIX"_"$$PLATFORM
    TARGETDIR = ..\..\..\Bin\\$$COMPILERLIBINFIX"_"$$PLATFORM
    QMAKE_POST_LINK=copy /Y $$SOURCEDIR"\\"$$TARGET".dll" $$TARGETDIR"\\*.dll"
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

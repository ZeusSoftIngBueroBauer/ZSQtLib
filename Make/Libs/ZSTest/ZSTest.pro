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
DEFINES += ZSTESTDLL_EXPORTS

INCLUDEPATH += ../../../Include/Libs

LIBS += -L../../../Lib/$$COMPILERLIBINFIX"_"$$PLATFORM

CONFIG(release, release|debug) {
    LIBS += -lZSSys"Qt"$$QT_MAJOR_VERSION"_"$$COMPILERLIBINFIX"_"$$PLATFORM
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSys"Qt"$$QT_MAJOR_VERSION"_"$$COMPILERLIBINFIX"_"$$PLATFORM"_d"
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

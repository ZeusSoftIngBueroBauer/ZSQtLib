#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T15:57:36
#
#-------------------------------------------------

QT += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG(release, release|debug) {
    TARGET = ZSAppTestMakeHelloWorld
}
CONFIG(debug, release|debug) {
    TARGET = ZSAppTestMakeHelloWorldd
}

TEMPLATE = app

INCLUDEPATH += ../Include

DEFINES += __linux__
CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
}

VERSION = 4.6.3

CONFIG(release, release|debug) {
    MOC_DIR = /tmp/ZeusSoft/ZSQtLib/Moc/ZSAppTestMakeHelloWorld/Linux/Release
    OBJECTS_DIR = /tmp/ZeusSoft/ZSQtLib/Int/ZSAppTestMakeHelloWorld/Linux/Release
}
CONFIG(debug, release|debug) {
    MOC_DIR = /tmp/ZeusSoft/ZSQtLib/Moc/ZSAppTestMakeHelloWorld/Linux/Debug
    OBJECTS_DIR = /tmp/ZeusSoft/ZSQtLib/Int/ZSAppTestMakeHelloWorld/Linux/Debug
}
DESTDIR = /usr/local/ZeusSoft/ZSQtLib

SOURCES += \
    ../Source/Main.cpp

HEADERS  +=

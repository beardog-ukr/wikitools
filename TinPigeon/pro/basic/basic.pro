QT = core
QT += network

TEMPLATE = app

CONFIG += console

TARGET = TinPigeon

MOC_DIR = build/moc
OBJECTS_DIR = build/obj
DESTDIR = build/bin

INCLUDEPATH += "../../src/engine/cpp/"
LIBS += ../engine/build/bin/libTinPigeon.a

HEADERS =
HEADERS += ../../src/basic/cpp/TinPigeonApp.h

SOURCES =  ../../src/basic/cpp/main.cpp
SOURCES += ../../src/basic/cpp/TinPigeonApp.cpp

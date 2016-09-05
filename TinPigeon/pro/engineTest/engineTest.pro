QT = core
QT += network
QT += testlib

TEMPLATE = app

CONFIG += console

TARGET = engineTest

MOC_DIR = build/moc
OBJECTS_DIR = build/obj
DESTDIR = build/bin

INCLUDEPATH += "../../src/engine/cpp/"
LIBS += ../engine/build/bin/libTinPigeon.a

HEADERS =
HEADERS += ../../src/engineTest/cpp/TestBasicHttpReader.h
HEADERS += ../../src/engineTest/cpp/TestSimplePageReader.h
HEADERS += ../../src/engineTest/cpp/TestWikiCategoryReader.h
HEADERS += ../../src/engineTest/cpp/TestWikiExtLinksReader.h

SOURCES =  ../../src/engineTest/cpp/TestMain.cpp
SOURCES += ../../src/engineTest/cpp/TestBasicHttpReader.cpp
SOURCES += ../../src/engineTest/cpp/TestSimplePageReader.cpp
SOURCES += ../../src/engineTest/cpp/TestWikiCategoryReader.cpp
SOURCES += ../../src/engineTest/cpp/TestWikiExtLinksReader.cpp

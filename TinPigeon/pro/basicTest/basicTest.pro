QT = core
QT += network
QT += testlib

TEMPLATE = app

CONFIG += console

TARGET = basicTest

MOC_DIR = build/moc
OBJECTS_DIR = build/obj
DESTDIR = build/bin

#RESOURCES = ../../src/engineTest/rc/TestWikiCategoryReader.qrc

INCLUDEPATH += "../../src/basic/cpp/"
INCLUDEPATH += "../../src/engine/cpp/"
LIBS += ../engine/build/bin/libTinPigeon.a

INCLUDEPATH += "../../src/FiveCatsLogger/cpp/"
LIBS += "../../src/FiveCatsLogger/bin/libFiveCatsLogger.a"

HEADERS =
HEADERS += ../../src/basicTest/cpp/TestBasic.h

HEADERS += ../../src/basic/cpp/ActionBasic.h
HEADERS += ../../src/basic/cpp/ActionWikiCategoryLoader.h
HEADERS += ../../src/basic/cpp/ActionWikitextREChecker.h

SOURCES =  ../../src/basicTest/cpp/TestMain.cpp
SOURCES += ../../src/basicTest/cpp/TestActionWikitextREChecker.cpp
SOURCES += ../../src/basicTest/cpp/TestActionWikiCategoryLoader.cpp

SOURCES += ../../src/basic/cpp/ActionBasic.cpp
SOURCES += ../../src/basic/cpp/ActionWikiCategoryLoader.cpp
SOURCES += ../../src/basic/cpp/ActionWikitextREChecker.cpp




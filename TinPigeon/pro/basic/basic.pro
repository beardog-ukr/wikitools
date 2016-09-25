QT = core
QT += network

TEMPLATE = app

CONFIG += console

TARGET = TinPigeon

MOC_DIR = build/moc
OBJECTS_DIR = build/obj
DESTDIR = build/bin

INCLUDEPATH += "../../src/basic/cpp/"

INCLUDEPATH += "../../src/engine/cpp/"
LIBS += ../engine/build/bin/libTinPigeon.a

INCLUDEPATH += "../../src/FiveCatsLogger/cpp/"
LIBS += "../../src/FiveCatsLogger/bin/libFiveCatsLogger.a"

HEADERS =
HEADERS += ../../src/basic/cpp/ActionBasic.h
HEADERS += ../../src/basic/cpp/ActionAuthMetainfoLoader.h
HEADERS += ../../src/basic/cpp/ActionLoginPerformer.h
HEADERS += ../../src/basic/cpp/ActionTokenLoader.h
HEADERS += ../../src/basic/cpp/ActionWikiCategoryLoader.h
HEADERS += ../../src/basic/cpp/ActionWikitextREChecker.h
HEADERS += ../../src/basic/cpp/TinPigeonApp.h

SOURCES =  ../../src/basic/cpp/main.cpp
SOURCES += ../../src/basic/cpp/ActionBasic.cpp
SOURCES += ../../src/basic/cpp/ActionAuthMetainfoLoader.cpp
SOURCES += ../../src/basic/cpp/ActionLoginPerformer.cpp
SOURCES += ../../src/basic/cpp/ActionTokenLoader.cpp
SOURCES += ../../src/basic/cpp/ActionWikiCategoryLoader.cpp
SOURCES += ../../src/basic/cpp/ActionWikitextREChecker.cpp
SOURCES += ../../src/basic/cpp/TinPigeonApp.cpp

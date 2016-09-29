QT = core
QT += network

TEMPLATE = lib

CONFIG += console
CONFIG += static
CONFIG -= app_bundle

TARGET = TinPigeon

DESTDIR = build/bin
MOC_DIR = build/moc
OBJECTS_DIR = build/obj
RCC_DIR = build/rcc
UI_DIR = build/ui

INCLUDEPATH += "../../src/engine/cpp/"
LIBS += ../engine/build/bin/libTinPigeon.a

INCLUDEPATH += "../../src/FiveCatsLogger/cpp/"
LIBS += "../../src/FiveCatsLogger/bin/libFiveCatsLogger.a"

HEADERS =
HEADERS += ../../src/engine/cpp/ArticleWikitextReader.h
HEADERS += ../../src/engine/cpp/AuthMetainfoLoader.h
HEADERS += ../../src/engine/cpp/BasicHttpReader.h
HEADERS += ../../src/engine/cpp/ErrorResponseProcessor.h
HEADERS += ../../src/engine/cpp/EditingPerformer.h
HEADERS += ../../src/engine/cpp/EditingTokenLoader.h
HEADERS += ../../src/engine/cpp/TokenLoader.h
HEADERS += ../../src/engine/cpp/LoginPerformer.h
HEADERS += ../../src/engine/cpp/WikiCategoryReader.h

SOURCES =
SOURCES += ../../src/engine/cpp/ArticleWikitextReader.cpp
SOURCES += ../../src/engine/cpp/AuthMetainfoLoader.cpp
SOURCES += ../../src/engine/cpp/BasicHttpReader.cpp
SOURCES += ../../src/engine/cpp/ErrorResponseProcessor.cpp
SOURCES += ../../src/engine/cpp/EditingPerformer.cpp
SOURCES += ../../src/engine/cpp/EditingTokenLoader.cpp
SOURCES += ../../src/engine/cpp/TokenLoader.cpp
SOURCES += ../../src/engine/cpp/LoginPerformer.cpp
SOURCES += ../../src/engine/cpp/WikiCategoryReader.cpp

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

HEADERS =
HEADERS += ../../src/engine/cpp/BasicHttpReader.h
HEADERS += ../../src/engine/cpp/SimplePageReader.h
HEADERS += ../../src/engine/cpp/WikiCategoryReader.h

SOURCES =
SOURCES += ../../src/engine/cpp/BasicHttpReader.cpp
SOURCES += ../../src/engine/cpp/SimplePageReader.cpp
SOURCES += ../../src/engine/cpp/WikiCategoryReader.cpp

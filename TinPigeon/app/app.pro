TEMPLATE = app
TARGET = tinPigeon

QT = core
QT += network

CONFIG += debug
CONFIG += console

DESTDIR = build/dest
MOC_DIR = build/moc
OBJECTS_DIR = build/obj
RCC_DIR = build/rcc
UI_DIR = build/ui

HEADERS =  src/cpp/TinPigeonApp.h
HEADERS += src/cpp/BasicHttpReader.h
HEADERS += src/cpp/SimplePageReader.h

SOURCES =  src/cpp/main.cpp
SOURCES += src/cpp/TinPigeonApp.cpp
SOURCES += src/cpp/BasicHttpReader.cpp
SOURCES += src/cpp/SimplePageReader.cpp

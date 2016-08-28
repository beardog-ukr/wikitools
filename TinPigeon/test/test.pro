QT += core
QT += testlib

TEMPLATE = app

CONFIG += console

TARGET = "testApp"

MOC_DIR = build/moc
OBJECTS_DIR = build/obj
DESTDIR = build/bin

INCLUDEPATH += ../app/src/cpp

HEADERS = src/cpp/TestsssssssssssssssEngine.h
HEADERS += ../app/src/cpp/sssssssssssssssEngine.h


SOURCES =  src/cpp/TestMain.cpp
SOURCES += src/cpp/TestssssssssssssssssssEngine.cpp
SOURCES += ../app/src/cpp/ssssssssssssssEngine.cpp

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

HEADERS += \
           src/cpp/TinPigeonApp.h

SOURCES +=  \
           src/cpp/main.cpp \
           src/cpp/TinPigeonApp.cpp

#RESOURCES += \
#    layouts.qrc

QT = core
QT += network
QT += testlib

TEMPLATE = app

CONFIG += console

TARGET = engineTest

MOC_DIR = build/moc
OBJECTS_DIR = build/obj
DESTDIR = build/bin

RESOURCES = ../../src/engineTest/rc/TestWikiCategoryReader.qrc

INCLUDEPATH += "../../src/engine/cpp/"
LIBS += ../engine/build/bin/libTinPigeon.a

INCLUDEPATH += "../../src/FiveCatsLogger/cpp/"
LIBS += "../../src/FiveCatsLogger/bin/libFiveCatsLogger.a"

HEADERS =
HEADERS += ../../src/engineTest/cpp/TestEngine.h

SOURCES =  ../../src/engineTest/cpp/TestMain.cpp
SOURCES += ../../src/engineTest/cpp/TestEngine.cpp
SOURCES += ../../src/engineTest/cpp/TestArticleWikitextReader.cpp
SOURCES += ../../src/engineTest/cpp/TestErrorResponseProcessor.cpp
SOURCES += ../../src/engineTest/cpp/TestEditingTokenLoader.cpp
SOURCES += ../../src/engineTest/cpp/TestLoginPerformer.cpp
SOURCES += ../../src/engineTest/cpp/TestTokenLoader.cpp
SOURCES += ../../src/engineTest/cpp/TestWikiCategoryReader.cpp

#ifndef __TestEngine__
#define __TestEngine__

#include <QtTest/QtTest>

#include <QString>
#include <QByteArray>

class TestEngine: public QObject
{
  Q_OBJECT
private slots:
  void initTestCase();

  void TestWikiCategoryReader_readElementsFromJsonDoc_BasicA();
  void TestWikiCategoryReader_readElementsFromJsonDoc_Empty();

  void TestWikiCategoryReader_readErrorFromJsonDoc_NoError();
  void TestWikiCategoryReader_readErrorFromJsonDoc_Empty();
  void TestWikiCategoryReader_readErrorFromJsonDoc_BasicA();
  void TestWikiCategoryReader_readErrorFromJsonDoc_BadJsonA();

  void TestWikiCategoryReader_readContinueId_BasicA();
  void TestWikiCategoryReader_readContinueId_Final();
  void TestWikiCategoryReader_readContinueId_Empty();

private:
  QString correctShortResp;

  void readFile(const QString fileName, QByteArray& data) ;
};

#endif

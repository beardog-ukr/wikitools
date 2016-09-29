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

  void ArticleWikitextReader_readWikitextFromJson_BasicA();

  void ErrorResponseProcessor_loadJson_BasicA();
  void ErrorResponseProcessor_loadJson_NoError();

  void EditingTokenLoader_readValuesFromJson_Basic();

  void LoginPerformer_readValuesFromJson_Basic();

  void TokenLoader_readValuesFromJson_Basic();
  void TokenLoader_readValuesFromJson_Double();

  void TestWikiCategoryReader_readElementsFromJsonDoc_BasicA();
  void TestWikiCategoryReader_readElementsFromJsonDoc_Empty();

  void TestWikiCategoryReader_readContinueId_BasicA();
  void TestWikiCategoryReader_readContinueId_Final();
  void TestWikiCategoryReader_readContinueId_Empty();

private:
  QString correctShortResp;

  void readFile(const QString fileName, QByteArray& data) ;
};

#endif

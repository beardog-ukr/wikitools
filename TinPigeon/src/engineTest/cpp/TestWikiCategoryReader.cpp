#include "TestEngine.h"
#include "WikiCategoryReader.h"

#include <QFile>
#include <QTextStream>
#include <QJsonDocument>

// ============================================================================
// ============================================================================
// ============================================================================

void TestEngine::TestWikiCategoryReader_readElementsFromJsonDoc_BasicA() {
  QByteArray barr;
  readFile(":/TestWikiCategoryReader/ShortCorrectA.json", barr);

  QJsonDocument doc = QJsonDocument::fromJson(barr);
  QJsonObject obj = doc.object();
  QCOMPARE(obj.isEmpty(), false);

  WikiCategoryReader wcr;
  QStringList elements ;
  bool tmpb =  wcr.readElementsFromJsonDoc(&obj, elements);

  QCOMPARE(tmpb, true);
  QCOMPARE(elements.count(), 3);
  QCOMPARE(elements[0], QString("Andriy Biletsky (politician)"));
  QCOMPARE(elements[1], QString("Nadiya Savchenko"));
  QCOMPARE(elements[2], QString("Valentyn Zghursky"));
}

// ============================================================================

void TestEngine::TestWikiCategoryReader_readElementsFromJsonDoc_Empty() {
  QByteArray barr;
  readFile(":/TestWikiCategoryReader/Empty.json", barr);

  QJsonDocument doc = QJsonDocument::fromJson(barr);
  QJsonObject obj = doc.object();
  QCOMPARE(obj.isEmpty(), false);

  WikiCategoryReader wcr;
  QStringList elements ;
  bool tmpb =  wcr.readElementsFromJsonDoc(&obj, elements);

  QCOMPARE(tmpb, false);
  QCOMPARE(elements.count(), 0);
  QCOMPARE(wcr.errorMessage.isEmpty(), false);
}

// ============================================================================

//void TestEngine::TestWikiCategoryReader_readErrorFromJsonDoc_NoError() {
//  QByteArray barr;
//  readFile(":/TestWikiCategoryReader/ShortCorrectA.json", barr);

//  QJsonDocument doc = QJsonDocument::fromJson(barr);
//  QJsonObject obj = doc.object();
//  QCOMPARE(obj.isEmpty(), false);

//  WikiCategoryReader wcr;
//  QStringList elements ;
//  bool tmpb =  wcr.readErrorFromJsonDoc(&obj);

//  QCOMPARE(tmpb, false);
//  QCOMPARE(wcr.errorMessage.isEmpty(), true);
//}

// ============================================================================

//void TestEngine::TestWikiCategoryReader_readErrorFromJsonDoc_Empty() {
//  QByteArray barr;
//  readFile(":/TestWikiCategoryReader/ShortCorrectA.json", barr);

//  QJsonDocument doc = QJsonDocument::fromJson(barr);
//  QJsonObject obj = doc.object();
//  QCOMPARE(obj.isEmpty(), false);

//  WikiCategoryReader wcr;
//  QStringList elements ;
//  bool tmpb =  wcr.readErrorFromJsonDoc(&obj);

//  QCOMPARE(tmpb, false);
//  QCOMPARE(wcr.errorMessage.isEmpty(), true);
//}

// ============================================================================

//void TestEngine::TestWikiCategoryReader_readErrorFromJsonDoc_BasicA() {
//  QByteArray barr;
//  readFile(":/TestWikiCategoryReader/ErrorA.json", barr);

//  QJsonDocument doc = QJsonDocument::fromJson(barr);
//  QJsonObject obj = doc.object();
//  QCOMPARE(obj.isEmpty(), false);

//  WikiCategoryReader wcr;
//  QStringList elements ;
//  bool tmpb =  wcr.readErrorFromJsonDoc(&obj);

//  QCOMPARE(tmpb, true);
//  QCOMPARE(wcr.errorMessage,
//           QString("Received error: The category name you entered is not valid (cminvalidcategory)"));
//}

// ============================================================================

//void TestEngine::TestWikiCategoryReader_readErrorFromJsonDoc_BadJsonA() {
//  //TODO: implement later, maybe use error message for wikitext
//}

// ============================================================================

void TestEngine::TestWikiCategoryReader_readContinueId_BasicA() {
  QByteArray barr;
  readFile(":/TestWikiCategoryReader/LongFirstA.json", barr);

  QJsonDocument doc = QJsonDocument::fromJson(barr);
  QJsonObject obj = doc.object();
  QCOMPARE(obj.isEmpty(), false);

  WikiCategoryReader wcr;
  bool tmpb = wcr.readContinueId(&obj);

  QCOMPARE(tmpb, true);
  QCOMPARE(wcr.nextContinueId, QString("page|3d43412d434104433d573f45372b0433273f2f4b0427412d04452749273d573f45372b0433273f2f4b04272b4d0412822e110133018f808f7f8f7d8f7c8f818f08|8891230"));
  QCOMPARE(wcr.errorMessage.isEmpty(), true);
}

// ============================================================================

void TestEngine::TestWikiCategoryReader_readContinueId_Final() {
  QByteArray barr;
  readFile(":/TestWikiCategoryReader/ShortCorrectA.json", barr);

  QJsonDocument doc = QJsonDocument::fromJson(barr);
  QJsonObject obj = doc.object();
  QCOMPARE(obj.isEmpty(), false);

  WikiCategoryReader wcr;
  bool tmpb = wcr.readContinueId(&obj);

  QCOMPARE(tmpb, true);
  QCOMPARE(wcr.nextContinueId.isEmpty(), true);
  QCOMPARE(wcr.errorMessage.isEmpty(), true);
}

// ============================================================================

void TestEngine::TestWikiCategoryReader_readContinueId_Empty() {
  QByteArray barr;
  readFile(":/TestWikiCategoryReader/Empty.json", barr);

  QJsonDocument doc = QJsonDocument::fromJson(barr);
  QJsonObject obj = doc.object();
  QCOMPARE(obj.isEmpty(), false);

  WikiCategoryReader wcr;
  bool tmpb = wcr.readContinueId(&obj);

  QCOMPARE(tmpb, true);
  QCOMPARE(wcr.nextContinueId.isEmpty(), true);
  QCOMPARE(wcr.errorMessage.isEmpty(), true);
}

// ============================================================================
// ============================================================================

#include "TestEngine.h"
#include "TokenLoader.h"

#include <QFile>
#include <QTextStream>
#include <QJsonDocument>

// ============================================================================
// ============================================================================
// ============================================================================

void TestEngine::TokenLoader_readValuesFromJson_Basic() {
  QByteArray barr;
  readFile(":/TokenLoader/Basic.json", barr);

  QJsonDocument doc = QJsonDocument::fromJson(barr);
  QJsonObject obj = doc.object();
  QCOMPARE(obj.isEmpty(), false);

  TokenLoader tl;
  tl.addTokenType("login");
  bool tmpb =  tl.readValuesFromJson(&obj);
  QCOMPARE(tmpb, true);

  QCOMPARE(tl.values.count(), 1 );
  QCOMPARE(tl.errorMessage.isEmpty(), true);

  QCOMPARE(tl.getToken("login").compare("a90988b26a383133d9ee1dab82e8a96a57e51193+\\"), 0 );
}

// ============================================================================

void TestEngine::TokenLoader_readValuesFromJson_Double() {
  QByteArray barr;
  readFile(":/TokenLoader/Double.json", barr);

  QJsonDocument doc = QJsonDocument::fromJson(barr);
  QJsonObject obj = doc.object();
  QCOMPARE(obj.isEmpty(), false);

  TokenLoader tl;
  tl.addTokenType("login");
  tl.addTokenType("watch");
  bool tmpb =  tl.readValuesFromJson(&obj);
  QCOMPARE(tmpb, true);

  QCOMPARE(tl.values.count(), 2 );
  QCOMPARE(tl.errorMessage.isEmpty(), true);

  QCOMPARE(tl.getToken("login").compare("0a6aff02deda3370d263fbf6791e95f057e51296+\\"), 0 );
  QCOMPARE(tl.getToken("watch").compare("4115baf11da4071426704a3f60c0ec8657e51296+\\"), 0 );
}



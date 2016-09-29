#include "TestEngine.h"
#include "LoginPerformer.h"

#include <QFile>
#include <QTextStream>
#include <QJsonDocument>

// ============================================================================
// ============================================================================
// ============================================================================

void TestEngine::LoginPerformer_readValuesFromJson_Basic() {
  QByteArray barr;
  readFile(":/LoginPerformer/Basic.json", barr);

  QJsonDocument doc = QJsonDocument::fromJson(barr);
  QJsonObject obj = doc.object();
  QCOMPARE(obj.isEmpty(), false);

  LoginPerformer lp;
  bool tmpb =  lp.readValuesFromJson(&obj);
  QCOMPARE(tmpb, true);

  QCOMPARE(lp.errorMessage.isEmpty(), true);

  QCOMPARE(lp.getLgToken().compare("0b34144e12e4df1ed9137829594b8e11"), 0 );
}

// ============================================================================
// ============================================================================



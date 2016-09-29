#include "TestEngine.h"
#include "EditingTokenLoader.h"

#include <QFile>
#include <QTextStream>
#include <QJsonDocument>

// ============================================================================
// ============================================================================
// ============================================================================

void TestEngine::EditingTokenLoader_readValuesFromJson_Basic() {
  QByteArray barr;
  readFile(":/EditTokenLoader/Basic.json", barr);

  QJsonDocument doc = QJsonDocument::fromJson(barr);
  QJsonObject obj = doc.object();
  QCOMPARE(obj.isEmpty(), false);

  EditingTokenLoader etl;
  bool tmpb =  etl.readValuesFromJson(&obj);
  QCOMPARE(tmpb, true);

  QCOMPARE(etl.errorMessage.isEmpty(), true);
  QCOMPARE(etl.getTimestamp().compare("2016-08-23T22:45:26Z"), 0 );
  QCOMPARE(etl.getToken().compare("36d04feffe8093d08f9f11ca2b9fdf3057e929e5+\\"), 0 );

}

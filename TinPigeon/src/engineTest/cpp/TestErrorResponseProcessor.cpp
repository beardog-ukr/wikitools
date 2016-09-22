#include "TestEngine.h"
#include "ErrorResponseProcessor.h"

#include <QFile>
#include <QTextStream>
#include <QJsonDocument>

// ============================================================================
// ============================================================================
// ============================================================================

void TestEngine::ErrorResponseProcessor_loadJson_BasicA() {
  QByteArray barr;
  readFile(":/TestWikiCategoryReader/ErrorA.json", barr);

  QJsonDocument doc = QJsonDocument::fromJson(barr);
  QJsonObject obj = doc.object();
  QCOMPARE(obj.isEmpty(), false);

  ErrorResponseProcessor erp;
  erp.loadJson(&obj);

  QCOMPARE(erp.hasError(), true);
  QCOMPARE(erp.errorCode(), QString("cminvalidcategory") );
  QCOMPARE(erp.errorInfo(), QString("The category name you entered is not valid"));
}

// ============================================================================

void TestEngine::ErrorResponseProcessor_loadJson_NoError() {
  QByteArray barr;
  readFile(":/TestWikiCategoryReader/ShortCorrectA.json", barr);

  QJsonDocument doc = QJsonDocument::fromJson(barr);
  QJsonObject obj = doc.object();
  QCOMPARE(obj.isEmpty(), false);

  ErrorResponseProcessor erp;
  erp.loadJson(&obj);

  QCOMPARE(erp.hasError(), false);
  QCOMPARE(erp.errorCode().isEmpty(), true );
  QCOMPARE(erp.errorInfo().isEmpty(), true );
}

// ============================================================================
// ============================================================================


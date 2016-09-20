#include "TestEngine.h"
#include "WikiCategoryReader.h"

#include <QFile>
#include <QTextStream>
#include <QJsonDocument>

// ============================================================================
// ============================================================================
// ============================================================================

void TestEngine::initTestCase() {
  correctShortResp = "";


}

// ============================================================================

void TestEngine::readFile(const QString fileName, QByteArray& ba) {
  QFile data(fileName);
  if ( !data.open(QFile::ReadOnly)) {
    QFAIL( "Problem opening file ()" );
  }

  ba.clear();
  ba.append(data.readAll());

  data.close();
}

// ============================================================================

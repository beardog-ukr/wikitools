#include "TestEngine.h"
#include "ArticleWikitextReader.h"

#include <QFile>
#include <QTextStream>
#include <QJsonDocument>

// ============================================================================
// ============================================================================
// ============================================================================

void TestEngine::ArticleWikitextReader_readWikitextFromJson_BasicA() {
  QByteArray barr;
  readFile(":/TestArticleWikitextReader/Basic.json", barr);

  QJsonDocument doc = QJsonDocument::fromJson(barr);
  QJsonObject obj = doc.object();
  QCOMPARE(obj.isEmpty(), false);

  ArticleWikitextReader awtr;
  QStringList elements ;
  bool tmpb =  awtr.readWikitextFromJson(&obj);

  QCOMPARE(tmpb, true);

  QString exp = "'''Ella Smith''' (born 6 June 1983) is an English actress.\n" ;
  exp += "[[Category:National Youth Theatre members]]\n\n{{UK-actor-stub}}";

  QCOMPARE( awtr.wikitext, exp );
  QCOMPARE(awtr.errorMessage.isEmpty(), true);
}

// ============================================================================

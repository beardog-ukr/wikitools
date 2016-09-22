#include "TestBasic.h"
#include "ActionWikitextREChecker.h"
#include "FiveCatsLogger.h"

#include <QFile>
#include <QRegularExpression>
#include <QTextStream>
#include <QJsonDocument>

// ============================================================================
// ============================================================================
// ============================================================================

void TestBasic::ActionWikitextREChecker_checkForRE_OneMatch() {

  ActionWikitextREChecker rech;
//  FiveCatsLogger* c5 = new FiveCatsLogger();
//  c5->setLogLevel(FiveCatsLogger::Trace);
//  rech.setLogger(c5);

  QString wt = "Whatever happens\n";
  wt += "we have got\n";
  wt += "The Maxim gun\n";
  wt += "and they shall not\n";

  QRegularExpression re("Maxim .*");
  rech.regExp = re;

  bool tmpb = rech.checkForRE("Title", wt);
  QCOMPARE(tmpb, true);

  QCOMPARE(rech.matches.count(), 1);
  QPair<QString, QString> p = rech.matches.at(0);
  QCOMPARE(p.first, QString("Title") );
  QCOMPARE(p.second, QString("The Maxim gun") ) ;

//  rech.setLogger(0);
//  delete c5;
}

// ============================================================================

void TestBasic::ActionWikitextREChecker_checkForRE_TwoMatches() {
  ActionWikitextREChecker rech;

  QString wt = "Whatever happens\n";
  wt += "we have got\n";
  wt += "The Maxim gun\n";
  wt += "and they shall not\n";

  QRegularExpression re("^.*ot$");
  rech.regExp = re;

  bool tmpb = rech.checkForRE("TwoMatches", wt);
  QCOMPARE(tmpb, true);

  QCOMPARE(rech.matches.count(), 2);
  QPair<QString, QString> p0 = rech.matches.at(0);
  QCOMPARE(p0.first, QString("TwoMatches") );
  QCOMPARE(p0.second, QString("we have got") ) ;
  QPair<QString, QString> p1 = rech.matches.at(1);
  QCOMPARE(p1.first, QString("TwoMatches") );
  QCOMPARE(p1.second, QString("and they shall not") ) ;
}

// ============================================================================

void TestBasic::ActionWikitextREChecker_checkForRE_NoMatches() {
  ActionWikitextREChecker rech;

  QString wt = "Whatever happens\n";
  wt += "we have got\n";
  wt += "The Maxim gun\n";
  wt += "and they shall not\n";

  QRegularExpression re("^.*444555555$");
  rech.regExp = re;

  bool tmpb = rech.checkForRE("NoMatches", wt);
  QCOMPARE(tmpb, true);

  QCOMPARE(rech.matches.count(), 0);
  QCOMPARE(rech.errorMessage.isEmpty(), true );
}

// ============================================================================

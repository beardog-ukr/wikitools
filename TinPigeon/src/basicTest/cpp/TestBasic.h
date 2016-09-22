#ifndef __TestBasic__
#define __TestBasic__

#include <QtTest/QtTest>

#include <QString>
#include <QByteArray>

class TestBasic: public QObject
{
  Q_OBJECT
private slots:
  void ActionWikitextREChecker_checkForRE_OneMatch();
  void ActionWikitextREChecker_checkForRE_TwoMatches();
  void ActionWikitextREChecker_checkForRE_NoMatches();

  void ActionWikiCategoryLoader_dummy_A();


 
};

#endif

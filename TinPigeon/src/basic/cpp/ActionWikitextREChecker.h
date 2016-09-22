#ifndef __ActionWikitextREChecker__
#define __ActionWikitextREChecker__

class ArticleWikitextReader;
class FiveCatsLogger;
#include "ActionBasic.h"

// === =======================================================================

#include <QList>
#include <QPair>
#include <QRegularExpression>
#include <QStringList>
class QTimer;
class QNetworkAccessManager;
class QCommandLineParser;

// === =======================================================================

class ActionWikitextREChecker : public ActionBasic {
  Q_OBJECT
  friend class TestBasic;
public:
  ActionWikitextREChecker(QObject *parent = 0);
  virtual ~ActionWikitextREChecker();

  static void initCommandLineParser(QCommandLineParser& parser);
  virtual bool loadCommandLineParser(const QCommandLineParser& parser);

  virtual bool startAction();

protected slots:
  void processResult();

protected:
  QString titlesFileName;
  QString reportFileName;

  QString wikiLanguage ;

  bool loadPageTitles();
  QStringList titlesToProcess;

  QRegularExpression regExp;
  bool checkForRE(const QString& title, const QString& data);

  QList<QPair<QString, QString> > matches;
  QStringList processedTitles;

  ArticleWikitextReader* pageReader;
  QNetworkAccessManager* networkAccessManager;

  bool startNextTitle();
  void processFinalResult();
};

#endif

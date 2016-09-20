#ifndef __ActionWikiCategoryLoader__
#define __ActionWikiCategoryLoader__

class WikiCategoryReader;
class FiveCatsLogger;
#include "ActionBasic.h"

// === =======================================================================

#include <QStringList>
class QTimer;
class QNetworkAccessManager;
class QCommandLineParser;

// === =======================================================================

class ActionWikiCategoryLoader : public ActionBasic {
  Q_OBJECT
//  friend class TestBasic;
public:
  ActionWikiCategoryLoader(QObject *parent = 0);
  virtual ~ActionWikiCategoryLoader();

  static void initCommandLineParser(QCommandLineParser& parser);
  virtual bool loadCommandLineParser(const QCommandLineParser& parser);

  virtual bool startAction();

protected slots:
  void processResult();

protected:
  void printResultList() ;
  bool saveResultList() ;

  QString categoryToLoad;
  QString wikiLanguage;
  QString resultFileName;

  QStringList categoryTitles;

  WikiCategoryReader* categoryReader;
  QNetworkAccessManager* networkAccessManager;
};

#endif

#include "ActionAuthMetainfoLoader.h"

#include "FiveCatsLogger.h"
#include "AuthMetainfoLoader.h"

// === =======================================================================

#include <QByteArray>
#include <QCommandLineParser>
#include <QFile>
#include <QNetworkAccessManager>

// === =======================================================================
// === =======================================================================

ActionAuthMetainfoLoader::ActionAuthMetainfoLoader(QObject *parent)
                         :ActionBasic(parent){
  wikiLanguage = "en";

  pageReader = 0 ;
  networkAccessManager = 0; //
}

// === =======================================================================

ActionAuthMetainfoLoader::~ActionAuthMetainfoLoader() {
  delete networkAccessManager;
  delete pageReader;
}

// === =======================================================================

void ActionAuthMetainfoLoader::initCommandLineParser(QCommandLineParser& parser) {
  // command line options

  QCommandLineOption langOption("lang",
      "Wiki language", "lang");
  parser.addOption(langOption) ;
}

// === =======================================================================

bool ActionAuthMetainfoLoader::loadCommandLineParser(const QCommandLineParser& parser) {
  QString tmps;

  tmps = parser.value("lang");
  if (!tmps.isEmpty()) {
    wikiLanguage = tmps ;
  }

  return true;
}

// === =======================================================================

bool ActionAuthMetainfoLoader::startAction() {
  networkAccessManager = new QNetworkAccessManager(this);
  c5t(c5, __c5_MN__, "Going to start");

  pageReader = new AuthMetainfoLoader(networkAccessManager, this);
  pageReader->setLogger(c5);
  pageReader->setWiki(wikiLanguage);
  connect(pageReader, SIGNAL(finished()), this, SLOT(processResult()) ) ;

  bool started = pageReader->start();
  if (!started) {
    errorMessage = pageReader->getErrorMessage();
    delete pageReader;
    pageReader = 0;
  }

  return started;
}

// === =======================================================================

void ActionAuthMetainfoLoader::processResult() {
  const QString em = pageReader->getErrorMessage();
  if (!em.isEmpty()) {
    errorMessage = em;
    emit actionFailure();
    return;
  }

  QString rStr = pageReader->getAllResponse();

  QStringList sl = rStr.split("\n");


  c5i(c5, "Got following response:");
  QStringList::const_iterator pti = sl.constBegin();
  for (; pti!=sl.constEnd(); pti++) {
    c5i(c5, *pti );
  }
  c5i(c5, "= = = = = = = = = = = = = = =");

  emit actionDone();
}

// === =======================================================================
// === =======================================================================



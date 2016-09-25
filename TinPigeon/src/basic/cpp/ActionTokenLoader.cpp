#include "ActionTokenLoader.h"

#include "FiveCatsLogger.h"
#include "TokenLoader.h"

// === =======================================================================

#include <QByteArray>
#include <QCommandLineParser>
#include <QFile>
#include <QNetworkAccessManager>

// === =======================================================================
// === =======================================================================

ActionTokenLoader::ActionTokenLoader(QObject *parent)
                  :ActionBasic(parent){
  wikiLanguage = "en";
  tokenType = "login";

  tokenLoader = 0 ;
  networkAccessManager = 0; //
}

// === =======================================================================

ActionTokenLoader::~ActionTokenLoader() {
  delete networkAccessManager;
  delete tokenLoader;
}

// === =======================================================================

void ActionTokenLoader::initCommandLineParser(QCommandLineParser& parser) {
  // command line options

  QCommandLineOption langOption("lang", "Wiki language", "lang");
  parser.addOption(langOption) ;

  parser.addOption(QCommandLineOption("type",
    "Token action type (login by default)", "type")) ;
}

// === =======================================================================

bool ActionTokenLoader::loadCommandLineParser(const QCommandLineParser& parser) {
  QString tmps;

  tmps = parser.value("lang");
  if (!tmps.isEmpty()) {
    wikiLanguage = tmps ;
  }

  tmps = parser.value("type");
  if (!tmps.isEmpty()) {
    tokenType = tmps ;
  }

  return true;
}

// === =======================================================================

bool ActionTokenLoader::startAction() {
  networkAccessManager = new QNetworkAccessManager(this);
  c5t(c5, __c5_MN__, "Going to start");

  tokenLoader = new TokenLoader(networkAccessManager, this);
  tokenLoader->setLogger(c5);
  tokenLoader->setWiki(wikiLanguage);
  tokenLoader->addTokenType(tokenType);
  connect(tokenLoader, SIGNAL(finished()), this, SLOT(processResult()) ) ;

  bool started = tokenLoader->start();
  if (!started) {
    errorMessage = tokenLoader->getErrorMessage();
    delete tokenLoader;
    tokenLoader = 0;
  }

  return started;
}

// === =======================================================================

void ActionTokenLoader::processResult() {
  const QString em = tokenLoader->getErrorMessage();
  if (!em.isEmpty()) {
    errorMessage = em;
    emit actionFailure();
    return;
  }

  token = tokenLoader->getToken(tokenType);

  c5i(c5, QString("Requested \"%1\" token value is \"%2\"")
             .arg(tokenType).arg(token) );

  emit actionDone();
}

// === =======================================================================
// === =======================================================================



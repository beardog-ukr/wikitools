#include "ActionLoginPerformer.h"

#include "FiveCatsLogger.h"
#include "LoginPerformer.h"
#include "TokenLoader.h"

// === =======================================================================

#include <QByteArray>
#include <QCommandLineParser>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>

// === =======================================================================
// === =======================================================================

ActionLoginPerformer::ActionLoginPerformer(QObject *parent)
                     :ActionBasic(parent){
  wikiLanguage = "en";

  tokenLoader = 0 ;
  loginPerformer = 0;
  networkAccessManager = 0; //
}

// === =======================================================================

ActionLoginPerformer::~ActionLoginPerformer() {
  delete networkAccessManager ;
  delete tokenLoader ;
  delete loginPerformer ;
}

// === =======================================================================

void ActionLoginPerformer::initCommandLineParser(QCommandLineParser& parser) {
  // command line options

  QCommandLineOption langOption("lang", "Wiki language", "lang");
  parser.addOption(langOption) ;

//  parser.addOption(QCommandLineOption("",
//    "Token action type (login by default)", "type")) ;
}

// === =======================================================================

bool ActionLoginPerformer::loadCommandLineParser(const QCommandLineParser& parser) {
  QString tmps;

  tmps = parser.value("lang");
  if (!tmps.isEmpty()) {
    wikiLanguage = tmps ;
  }

//  tmps = parser.value("type");
//  if (!tmps.isEmpty()) {
//    tokenType = tmps ;
//  }

  return true;
}

// === =======================================================================

bool ActionLoginPerformer::startAction() {
  networkAccessManager = new QNetworkAccessManager(this);
  networkAccessManager->setCookieJar(new QNetworkCookieJar());
  c5t(c5, __c5_MN__, "Going to start");

  tokenLoader = new TokenLoader(networkAccessManager, this);
  tokenLoader->setLogger(c5);
  tokenLoader->setWiki(wikiLanguage);
  tokenLoader->addTokenType("login");
  connect(tokenLoader, SIGNAL(finished()), this, SLOT(processTokenLoaded()) ) ;

  bool started = tokenLoader->start();
  if (!started) {
    errorMessage = tokenLoader->getErrorMessage();
    delete tokenLoader;
    tokenLoader = 0;
  }

  return started;
}

// === =======================================================================

void ActionLoginPerformer::processTokenLoaded() {
  const QString em = tokenLoader->getErrorMessage();
  if (!em.isEmpty()) {
    errorMessage = em;
    emit actionFailure();
    return;
  }

  const QString token = tokenLoader->getToken("login");
  const QVariant cookieHeader = tokenLoader->getCookieHeader();
  c5i(c5, QString("Requested \"%1\" token value is \"%2\"")
             .arg("login").arg(token) );
  delete tokenLoader;
  tokenLoader = 0;

  //now start login performer class
  loginPerformer = new LoginPerformer(networkAccessManager, this);
  loginPerformer->setWiki(wikiLanguage);
  loginPerformer->setLoginToken(token);
  loginPerformer->setLogger(c5);
  loginPerformer->setUserName("BearboarBot");
  loginPerformer->setUserPassword("WoolCoat");
  loginPerformer->setCookieHeader(cookieHeader);
  connect(loginPerformer, SIGNAL(finished()), this, SLOT(processResult()) ) ;

  bool started = loginPerformer->start();
  if (!started) {
    errorMessage = loginPerformer->getErrorMessage();
    delete loginPerformer;
    loginPerformer = 0;
  }
  else {
    c5d(c5, __c5_MN__, "Started logging in");
  }

//  emit actionDone();
}

// === =======================================================================

void ActionLoginPerformer::processResult() {
  const QString em = loginPerformer->getErrorMessage();
  if (!em.isEmpty()) {
    errorMessage = em;
    emit actionFailure();
//    return;
  }

  QString rStr = loginPerformer->getAllResponse();
  delete loginPerformer ;
  loginPerformer = 0;

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



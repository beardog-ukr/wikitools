#include "ActionReplaceEditor.h"

#include "FiveCatsLogger.h"
#include "ArticleWikitextReader.h"
#include "LoginPerformer.h"
#include "TokenLoader.h"
#include "EditingTokenLoader.h"

// === =======================================================================

#include <QByteArray>
#include <QCommandLineParser>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>

// === =======================================================================
// === =======================================================================

ActionReplaceEditor::ActionReplaceEditor(QObject *parent)
                     :ActionBasic(parent){
  wikiLanguage = "en";

  tokenLoader = 0 ;
  loginPerformer = 0;
  networkAccessManager = 0; //
}

// === =======================================================================

ActionReplaceEditor::~ActionReplaceEditor() {
  delete networkAccessManager ;
  delete tokenLoader ;
  delete loginPerformer ;
}

// === =======================================================================

void ActionReplaceEditor::initCommandLineParser(QCommandLineParser& parser) {
  // command line options

  QCommandLineOption langOption("lang", "Wiki language", "lang");
  parser.addOption(langOption) ;

  parser.addOption(QCommandLineOption("page", "Page to test (one page title)", "page")) ;
  parser.addOption(QCommandLineOption("in", "String to search for", "in")) ;
  parser.addOption(QCommandLineOption("out", "String to insert instead", "out")) ;
}

// === =======================================================================

bool ActionReplaceEditor::loadCommandLineParser(const QCommandLineParser& parser) {
  QString tmps;

  tmps = parser.value("lang");
  if (!tmps.isEmpty()) {
    wikiLanguage = tmps ;
  }

  pageTitle = parser.value("page");
  inStr = parser.value("in");
  outStr = parser.value("out");

  if (pageTitle.isEmpty()) {
    return false;
  }

  return true;
}

// === =======================================================================

bool ActionReplaceEditor::startAction() {
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

void ActionReplaceEditor::processTokenLoaded() {
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
  connect(loginPerformer, SIGNAL(finished()), this, SLOT(processLoggedIn()) ) ;

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

void ActionReplaceEditor::processLoggedIn() {
  const QString em = loginPerformer->getErrorMessage();
  if (!em.isEmpty()) {
    errorMessage = em;
    emit actionFailure();
    return;
  }

  QString loginToken = loginPerformer->getLgToken() ;
  if (loginToken.isEmpty()) {
    errorMessage = "Login token is empty" ;
    emit actionFailure();
    return;
  }

  //QString rStr = loginPerformer->getAllResponse();
  delete loginPerformer ;
  loginPerformer = 0;

//  QStringList sl = rStr.split("\n");
//  c5i(c5, "Got following response:");
//  QStringList::const_iterator pti = sl.constBegin();
//  for (; pti!=sl.constEnd(); pti++) {
//    c5i(c5, *pti );
//  }
//  c5i(c5, "= = = = = = = = = = = = = = =");

  wikitextLoader = new ArticleWikitextReader(networkAccessManager, this);
  wikitextLoader->setLogger(c5);
  wikitextLoader->setWiki(wikiLanguage);
  wikitextLoader->setPageTitle(pageTitle);
  connect(wikitextLoader, SIGNAL(finished()), this, SLOT(processWikitextLoaded()) ) ;


//  emit actionDone();
}

// === =======================================================================

void ActionReplaceEditor::processWikitextLoaded()  {
  const QString em = wikitextLoader->getErrorMessage();
  if (!em.isEmpty()) {
    errorMessage = em;
    emit actionFailure();
    return;
  }

  QString articleText = wikitextLoader->getWikitext() ;
  delete wikitextLoader ;
  wikitextLoader = 0;

  if (articleText.contains(inStr)) {
    articleText.replace(inStr, outStr) ;
  }


}

// === =======================================================================
// === =======================================================================



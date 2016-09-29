#include "ActionSetPageContent.h"

#include "FiveCatsLogger.h"
#include "EditingPerformer.h"
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

ActionSetPageContent::ActionSetPageContent(QObject *parent)
                     :ActionBasic(parent){
  wikiLanguage = "en";

  editor = 0 ;
  editingTokenLoader = 0;
  tokenLoader = 0 ;
  loginPerformer = 0;
  networkAccessManager = 0; //
}

// === =======================================================================

ActionSetPageContent::~ActionSetPageContent() {
  delete networkAccessManager ;
  delete tokenLoader ;
  delete loginPerformer ;
  delete editor;
  delete editingTokenLoader ;
}

// === =======================================================================

void ActionSetPageContent::initCommandLineParser(QCommandLineParser& parser) {
  // command line options

  QCommandLineOption langOption("lang", "Wiki language", "lang");
  parser.addOption(langOption) ;

  parser.addOption(QCommandLineOption("page", "Page to test (one page title)", "page")) ;
  parser.addOption(QCommandLineOption("tf", "Text file with page wikitext to set", "tf")) ;
  parser.addOption(QCommandLineOption("credentials", "A text file with login and password", "credentials")) ;
  parser.addOption(QCommandLineOption("summary", "Editing summary", "summary")) ;
}

// === =======================================================================

bool ActionSetPageContent::loadCommandLineParser(const QCommandLineParser& parser) {
  QString tmps;

  tmps = parser.value("lang");
  if (!tmps.isEmpty()) {
    wikiLanguage = tmps ;
  }

  pageTitle = parser.value("page") ;
  editSummary = parser.value("summary") ;

  //reading credentials
  tmps = parser.value("credentials");
  if (!readCredentials(tmps)) {
    return false;
  }

  tmps = parser.value("tf");
  if (!readNewText(tmps)) {
    return false;
  }

  if (pageTitle.isEmpty()) {
    return false;
  }

  return true;
}

// === =======================================================================

bool ActionSetPageContent::readNewText(const QString& fn) {
  QFile fl(fn);
  if ( !fl.open(QFile::ReadOnly)) {
    errorMessage = QString("Problem opening file (%1)").arg(fn);
    return false;
  }

  QTextStream stream(&fl);
  stream.setCodec( "UTF-8" );
//  QString line;
//  while (stream.readLineInto(&line)) {
//    edi.append(line);
//  }
  pageText = stream.readAll();

  fl.close();

  return true ;
}

// === =======================================================================

bool ActionSetPageContent::readCredentials(const QString& fn) {
  QFile fl(fn);
  if ( !fl.open(QFile::ReadOnly)) {
    errorMessage = QString("Problem opening file (%1)").arg(fn);
    return false;
  }

  QTextStream stream(&fl);
  stream.setCodec( "UTF-8" );
  if (! stream.readLineInto(&login)) {
    errorMessage = QString("Failed to read login from (%1)").arg(fn);
    return false;
  }
  if (! stream.readLineInto(&password)) {
    errorMessage = QString("Failed to read password from (%1)").arg(fn);
    return false;
  }

  fl.close();

  return true ;
}

// === =======================================================================

bool ActionSetPageContent::startAction() {
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

void ActionSetPageContent::processTokenLoaded() {
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
  loginPerformer->setUserName(login);
  loginPerformer->setUserPassword(password);
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

void ActionSetPageContent::processLoggedIn() {
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

  editingTokenLoader = new EditingTokenLoader(networkAccessManager, this);
  editingTokenLoader->setLogger(c5);
  editingTokenLoader->setWiki(wikiLanguage);
  editingTokenLoader->setPageTitle(pageTitle);
  connect(editingTokenLoader, SIGNAL(finished()), this, SLOT(processEditTokenReceived()) ) ;

  bool started = editingTokenLoader->start();
  if (!started) {
    errorMessage = editingTokenLoader->getErrorMessage();
    delete editingTokenLoader;
    editingTokenLoader = 0;
  }
  else {
    c5d(c5, __c5_MN__, "Requested edit token");
  }

//  emit actionDone();
}

// === =======================================================================

void ActionSetPageContent::processEditTokenReceived() {
  const QString em = editingTokenLoader->getErrorMessage();
  if (!em.isEmpty()) {
    errorMessage = em;
    emit actionFailure();
    return;
  }

  editingToken = editingTokenLoader->getToken() ;
  pageTimestamp = editingTokenLoader->getTimestamp() ;

  if (editingToken.isEmpty() || pageTimestamp.isEmpty()) {
    errorMessage = "Something wrong with edit tokens" ;
    emit actionFailure();
    return;
  }

  //QString rStr = loginPerformer->getAllResponse();
  delete editingTokenLoader ;
  editingTokenLoader = 0;

  // Finally, create a page editor
  editor = new EditingPerformer(networkAccessManager, this);
  editor->setLogger(c5);

  editor->setEditToken(editingToken);
  editor->setNewtext(pageText);
  editor->setSummary(editSummary);
  editor->setPageTitle(pageTitle);
  editor->setTimestamp(pageTimestamp);
  editor->setWiki(wikiLanguage);

  connect(editor, SIGNAL(finished()), this, SLOT(processPageEditComplete()) ) ;

  bool started = editor->start();
  if (!started) {
    errorMessage = editor->getErrorMessage();
    delete editor;
    editor = 0;
  }
  else {
    c5d(c5, __c5_MN__, "Started editing");
  }
}

// === =======================================================================

void ActionSetPageContent::processPageEditComplete() {
  const QString em = editor->getErrorMessage();
  if (!em.isEmpty()) {
    errorMessage = em;
    emit actionFailure();
    return;
  }

  //QString rStr = loginPerformer->getAllResponse();
  delete editor ;
  editor = 0;

  emit actionDone();
}

// === =======================================================================
// === =======================================================================



#include "TinPigeonApp.h"
#include "ActionBasic.h"
#include "ActionAuthMetainfoLoader.h"
#include "ActionLoginPerformer.h"
#include "ActionWikiCategoryLoader.h"
#include "ActionWikitextREChecker.h"
#include "ActionTokenLoader.h"
#include "ActionSetPageContent.h"

// === =======================================================================

#include <QCommandLineParser>
#include <QMap>
#include <QNetworkAccessManager>
#include <QTimer>

// === =======================================================================

#include "FiveCatsLogger.h"
#include "WikiCategoryReader.h"

// === =======================================================================

TinPigeonApp::TinPigeonApp(QObject* parent)
             :QObject(parent){
  startupTimer = 0;
  c5 = new FiveCatsLogger(FiveCatsLogger::Info);
  actionPerformer = 0;
}

// === =======================================================================

TinPigeonApp::~TinPigeonApp() {
  delete startupTimer;
  delete actionPerformer;
  delete c5;
}

// === =======================================================================

bool TinPigeonApp::processCommandLine() {
  bool result = true;
  QMap<QString, QString> options;

  QCommandLineParser parser;
  parser.setApplicationDescription("Tin Pigeon application");
  parser.addHelpOption();
  parser.addVersionOption();

  // command line options
  QCommandLineOption actionOption("action");
  actionOption.setDescription("Some action to perform (ami, wcl, recp)");
  actionOption.setValueName("action");
  parser.addOption(actionOption);

  c5->initCommandLineParser(parser);
  ActionWikiCategoryLoader::initCommandLineParser(parser);
  ActionWikitextREChecker::initCommandLineParser(parser);
  ActionAuthMetainfoLoader::initCommandLineParser(parser) ;
  ActionTokenLoader::initCommandLineParser(parser);
  ActionLoginPerformer::initCommandLineParser(parser) ;
  ActionSetPageContent::initCommandLineParser(parser) ;

  // Process the actual command line arguments given by the user
  QCoreApplication* ca = QCoreApplication::instance();
  parser.process(*ca);

  c5->loadCommandLineParser(parser);
  QString actionStr = parser.value(actionOption);

  if (actionStr == "ami" ) {
    actionPerformer = new ActionAuthMetainfoLoader(this);
  }
  else if (actionStr == "lp" ) {
    actionPerformer = new ActionLoginPerformer(this);
  }
  else if (actionStr == "wcl" ) {
    actionPerformer = new ActionWikiCategoryLoader(this);
  }
  else if (actionStr == "recp" ) {
    actionPerformer = new ActionWikitextREChecker(this);
  }
  else if (actionStr == "ltl" ) {
    actionPerformer = new ActionTokenLoader(this);
  }
  else if (actionStr == "spc" ) {
    actionPerformer = new ActionSetPageContent(this);
  }
  else if (actionStr.isEmpty() ) {
    c5w(c5, "Action must be specified");
    actionPerformer = 0;
    result = false;
  }

  if (result) {
    result = actionPerformer->loadCommandLineParser(parser);
  }
  else {
    c5i(c5,  parser.helpText() );
  }

  return result;
}

// === =======================================================================

bool TinPigeonApp::prepareToStart() {
  startupTimer = new QTimer(this) ;
  startupTimer->setSingleShot(true);
  connect(startupTimer, SIGNAL(timeout()),this, SLOT(startEveryting()) );
  startupTimer->start(500);
  return true;
}

// === =======================================================================

void TinPigeonApp::startEveryting() {
  delete startupTimer;
  startupTimer =0;

  actionPerformer->setLogger(c5);

  connect(actionPerformer, SIGNAL(actionDone()), this, SLOT(processActionResult()));
  connect(actionPerformer, SIGNAL(actionFailure()), this, SLOT(processActionFailure()));
  bool started = actionPerformer->startAction() ;
  if (!started) {
    c5c(c5, "ERROR: at start: " + actionPerformer->getErrorMessage() );
    delete actionPerformer;
    actionPerformer = 0;
  }
}

// === =======================================================================

void TinPigeonApp::processActionResult() {
  c5t(c5, __c5_MN__ , "here");

  const QString em = actionPerformer->getErrorMessage();
  if (!em.isEmpty()) {
    c5w(c5, __c5_MN__, "Action finished successfully, but with error message: " + em);
    QCoreApplication::exit(1);
  }

  QCoreApplication::exit(0);
}

// === =======================================================================

void TinPigeonApp::processActionFailure() {
  c5t(c5, __c5_MN__ , "here");

  const QString em = actionPerformer->getErrorMessage();
  if (em.isEmpty()) {
    c5w(c5, __c5_MN__, "ERROR: action failed for unknown reason");
  }
  else {
    c5w(c5, __c5_MN__, em);
  }

  QCoreApplication::exit(1);
}


// === =======================================================================

int TinPigeonApp::getAppExitCode() const {
  return appExitCode;
}

// === =======================================================================



// === =======================================================================
// === =======================================================================

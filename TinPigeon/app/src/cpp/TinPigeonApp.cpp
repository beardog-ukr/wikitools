#include "TinPigeonApp.h"

// === =======================================================================

#include <QDebug>

#include <QCommandLineParser>
#include <QMap>
#include <QNetworkAccessManager>
#include <QTimer>
//#include <math.h>       // modf
//using namespace std;

// === =======================================================================

#include "SimplePageReader.h"

// === =======================================================================
// === =======================================================================

TinPigeonApp::TinPigeonApp(QObject* parent)
             :QObject(parent){
  appExitCode=0;//
  urlToLoad = "";
  pageReader = 0;
  startupTimer = 0;
  networkAccessManager = new QNetworkAccessManager(this);
}

// === =======================================================================

TinPigeonApp::~TinPigeonApp() {
  delete startupTimer;
  delete networkAccessManager;
  delete pageReader;
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

  pageReader = new SimplePageReader(networkAccessManager, this);
  connect(pageReader, SIGNAL(finished()), this, SLOT(downloaderFinished()));

  bool result = pageReader->setUrlToLoad(urlToLoad);
  if (result) {
    result = pageReader->start() ;
  }

  if (!result) {
    qDebug() << "ERROR: at start: " << pageReader->getErrorMessage();
    delete pageReader;
    pageReader = 0;
  }
}

// === =======================================================================

bool TinPigeonApp::processCommandLine() {
  bool result = true;
  QMap<QString, QString> options;

  QCommandLineParser parser;
  parser.setApplicationDescription("Tin Pigeon application");
  parser.addHelpOption();
  parser.addVersionOption();

  // An option with a value
  QCommandLineOption urlOption("url");
  urlOption.setDescription("Url to download");
  urlOption.setValueName("url");
  parser.addOption(urlOption);

  // Process the actual command line arguments given by the user
  QCoreApplication* ca = QCoreApplication::instance();
  parser.process(*ca);

  urlToLoad = parser.value(urlOption);
  if (urlToLoad.isEmpty()) {
    result = false;
    qDebug() << "ERROR: url to download was not specified";
    result = false;
  }

  if (!result) {
    qDebug() << parser.helpText() ;
  }

  return result;
}

// === =======================================================================

int TinPigeonApp::getAppExitCode() const {
  return appExitCode;
}

// === =======================================================================

void TinPigeonApp::downloaderFinished() {
  qDebug() << "TinPigeonApp::downloaderFinished " << "here";

  const QString em = pageReader->getErrorMessage();
  if (em.isEmpty()) {
    qDebug() << "TinPigeonApp::downloaderFinished " << "got: ";
    qDebug() << pageReader->getReceivedData();
  }
  else {
    qDebug() << "TinPigeonApp::downloaderFinished " << "ERROR: " << em;
  }

  delete pageReader;
  pageReader = 0;

  QCoreApplication::exit(0);
}

// === =======================================================================

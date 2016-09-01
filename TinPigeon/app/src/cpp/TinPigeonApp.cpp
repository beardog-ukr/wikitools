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
  simplePageReader = 0;
  startupTimer = 0;
  networkAccessManager = new QNetworkAccessManager(this);
}

// === =======================================================================

TinPigeonApp::~TinPigeonApp() {
  delete startupTimer;
  delete networkAccessManager;
  delete simplePageReader;
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

  simplePageReader = new SimplePageReader(networkAccessManager, this);
  connect(simplePageReader, SIGNAL(finished()),
          this, SLOT(downloaderFinished()));

  bool result = simplePageReader->start(urlToLoad) ;
  if (!result) {
    qDebug() << "ERROR: at start: " << simplePageReader->getErrorMessage();
    delete simplePageReader;
    simplePageReader = 0;
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

  const QString em = simplePageReader->getErrorMessage();
  if (em.isEmpty()) {
    qDebug() << "TinPigeonApp::downloaderFinished " << "got: ";
    qDebug() << simplePageReader->getReceivedData();
  }
  else {
    qDebug() << "TinPigeonApp::downloaderFinished " << "ERROR: " << em;
  }

  delete simplePageReader;
  simplePageReader = 0;

  QCoreApplication::exit(0);
}

// === =======================================================================

#include "TinPigeonApp.h"

// === =======================================================================

#include <QDebug>

#include <QCommandLineParser>
#include <QMap>
#include <QNetworkAccessManager>
#include <QTimer>

// === =======================================================================

//#include "SimplePageReader.h"
#include "WikiCategoryReader.h"

// === =======================================================================
// === =======================================================================

TinPigeonApp::TinPigeonApp(QObject* parent)
             :QObject(parent){
  appExitCode=0;//
  wikiLanguage = "";
  categoryToLoad = "";
  categoryReader = 0;
  startupTimer = 0;
  networkAccessManager = new QNetworkAccessManager(this);
}

// === =======================================================================

TinPigeonApp::~TinPigeonApp() {
  delete startupTimer;
  delete networkAccessManager;
  delete categoryReader;
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
  QCommandLineOption categoryOption("category");
  categoryOption.setDescription("Wiki category to read");
  categoryOption.setValueName("category");
  parser.addOption(categoryOption);

  QCommandLineOption wikilangOption("wikilang");
  wikilangOption.setDescription("Wiki language (\"en\" will be used by default)");
  wikilangOption.setValueName("wikilang");
  parser.addOption(wikilangOption);

  // Process the actual command line arguments given by the user
  QCoreApplication* ca = QCoreApplication::instance();
  parser.process(*ca);

  categoryToLoad = parser.value(categoryOption);
  wikiLanguage = parser.value(wikilangOption);
  if (categoryToLoad.isEmpty()) {
    result = false;
    qDebug() << "ERROR: wiki category was not specified";
  }
  else {
    qDebug() << "category is " << categoryToLoad;
  }

  if (!result) {
    qDebug() << parser.helpText() ;
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

  categoryReader = new WikiCategoryReader(networkAccessManager, this);
  categoryReader->setCategory(categoryToLoad);
  if (!wikiLanguage.isEmpty()) {
    categoryReader->setWiki(wikiLanguage);
  }
  connect(categoryReader, SIGNAL(finished()), this, SLOT(processCategories()));

  bool result = categoryReader->start() ;

  if (!result) {
    qDebug() << "ERROR: at start: " << categoryReader->getErrorMessage();
    delete categoryReader;
    categoryReader = 0;
  }
}

// === =======================================================================

int TinPigeonApp::getAppExitCode() const {
  return appExitCode;
}

// === =======================================================================

void TinPigeonApp::processCategories() {
  qDebug() << "TinPigeonApp::processCategories " << "here";

  const QString em = categoryReader->getErrorMessage();
  if (!em.isEmpty()) {
    qDebug() << "TinPigeonApp::processCategories " << "ERROR: " << em;
    QCoreApplication::exit(1);
    return;
  }

  QList<WikiCategoryElement> wceList = categoryReader->getCategoryElements();
  qDebug() << "TinPigeonApp::processCategories " << "got " << wceList.count()
           << " elements"   ;

  QList<WikiCategoryElement>::const_iterator wceci;
  for (wceci = wceList.constBegin(); wceci != wceList.constEnd(); ++wceci) {
    qDebug() << "  " << wceci->pageId << "  as " << wceci->title;
  }

  delete categoryReader;
  categoryReader = 0;

  QCoreApplication::exit(0);
}

// === =======================================================================

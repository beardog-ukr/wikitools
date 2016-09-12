#include "TinPigeonApp.h"

// === =======================================================================

#include <QDebug>

#include <QCommandLineParser>
#include <QMap>
#include <QNetworkAccessManager>
#include <QTimer>

// === =======================================================================

#include "FiveCatsLogger.h"
#include "WikiCategoryReader.h"

// === =======================================================================
//http://www.sakhalit.com/node/394
//http://www.warheroes.ru/hero/hero.asp?Hero_id=1891
// === =======================================================================

TinPigeonApp::TinPigeonApp(QObject* parent)
             :QObject(parent){
  appExitCode=0;//
  wikiLanguage = "";
  categoryToLoad = "";
  categoryReader = 0;
  startupTimer = 0;
  networkAccessManager = new QNetworkAccessManager(this);
  c5 = new FiveCatsLogger(FiveCatsLogger::Info);
}

// === =======================================================================

TinPigeonApp::~TinPigeonApp() {
  delete startupTimer;
  delete networkAccessManager;
  delete categoryReader;
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
  QCommandLineOption categoryOption("category");
  categoryOption.setDescription("Wiki category to read");
  categoryOption.setValueName("category");
  parser.addOption(categoryOption);

  QCommandLineOption wikilangOption("wikilang");
  wikilangOption.setDescription("Wiki language (\"en\" will be used by default)");
  wikilangOption.setValueName("wikilang");
  parser.addOption(wikilangOption);

  c5->initCommandLineParser(parser);

  // Process the actual command line arguments given by the user
  QCoreApplication* ca = QCoreApplication::instance();
  parser.process(*ca);

  c5->loadCommandLineParser(parser);

  categoryToLoad = parser.value(categoryOption);
  wikiLanguage = parser.value(wikilangOption);
  if (categoryToLoad.isEmpty()) {
    result = false;
    c5c(c5, "ERROR: wiki category was not specified");
  }
  else {
    c5d(c5,  "category is " + categoryToLoad);
  }

  if (!result) {
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

  categoryReader = new WikiCategoryReader(networkAccessManager, this);
  categoryReader->setCategory(categoryToLoad);
  if (!wikiLanguage.isEmpty()) {
    categoryReader->setWiki(wikiLanguage);
  }
  connect(categoryReader, SIGNAL(finished()), this, SLOT(processCategories()));

  bool result = categoryReader->start() ;

  if (!result) {
    c5c(c5, "ERROR: at start: " + categoryReader->getErrorMessage() );
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
  c5d(c5, __c5_MN__ , "here");

  const QString em = categoryReader->getErrorMessage();
  if (!em.isEmpty()) {
    c5w(c5, __c5_MN__, "ERROR: " + em);
    QCoreApplication::exit(1);
    return;
  }

  QList<WikiCategoryElement> wceList = categoryReader->getCategoryElements();
  c5d(c5, __c5_MN__,  QString("got %1 elements").arg( wceList.count() ));

  QList<WikiCategoryElement>::const_iterator wceci;
  for (wceci = wceList.constBegin(); wceci != wceList.constEnd(); ++wceci) {
    c5d(c5, "  " + wceci->pageId + "  as " + wceci->title);
  }

  delete categoryReader;
  categoryReader = 0;

  QCoreApplication::exit(0);
}

// === =======================================================================

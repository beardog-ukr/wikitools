#include "ActionWikiCategoryLoader.h"

#include "FiveCatsLogger.h"
#include "WikiCategoryReader.h"

// === =======================================================================

#include <QByteArray>
#include <QCommandLineParser>
#include <QFile>
#include <QMap>
#include <QNetworkAccessManager>
#include <QTimer>

// === =======================================================================
// === =======================================================================

ActionWikiCategoryLoader::ActionWikiCategoryLoader(QObject *parent)
                         :ActionBasic(parent){
  wikiLanguage = "";
  categoryToLoad = "";
  resultFileName = "" ;
  categoryReader = 0;
  networkAccessManager = 0; //
}

// === =======================================================================

ActionWikiCategoryLoader::~ActionWikiCategoryLoader() {
  delete networkAccessManager;
  delete categoryReader;
}

// === =======================================================================

void ActionWikiCategoryLoader::initCommandLineParser(QCommandLineParser& parser) {
  // command line options
  QCommandLineOption categoryOption("wcl_category");
  categoryOption.setDescription("Wiki category to read");
  categoryOption.setValueName("wcl_category");
  parser.addOption(categoryOption);

  QCommandLineOption wikilangOption("wcl_lang");
  wikilangOption.setDescription("Wiki language (\"en\" will be used by default)");
  wikilangOption.setValueName("wcl_wikilang");
  parser.addOption(wikilangOption);

  QCommandLineOption fnOption("wcl_file");
  fnOption.setDescription("File to save the downloaded list (by default it will be printed to console)");
  fnOption.setValueName("wcl_file");
  parser.addOption(fnOption);
}

// === =======================================================================

bool ActionWikiCategoryLoader::loadCommandLineParser(const QCommandLineParser& parser) {
  QString tmps;

  tmps = parser.value("wcl_category");
  if (!tmps.isEmpty()) {
    categoryToLoad = tmps ;
  }

  tmps = parser.value("wcl_lang");
  if (!tmps.isEmpty()) {
    wikiLanguage = tmps ;
  }

  tmps = parser.value("wcl_file");
  if (!tmps.isEmpty()) {
    resultFileName = tmps ;
  }

  if (categoryToLoad.isEmpty()) {
    return false;
  }
  return true;
}

// === =======================================================================

bool ActionWikiCategoryLoader::startAction() {
  if (wikiLanguage.isEmpty()) {
    wikiLanguage = "en";
  }
  if (categoryToLoad.isEmpty()) {
    errorMessage = "Category was not specified before start";
    return false;
  }

  networkAccessManager = new QNetworkAccessManager(this);
  categoryReader = new WikiCategoryReader(networkAccessManager, this);
  categoryReader->setLogger(c5) ;
  categoryReader->setWiki(wikiLanguage) ;
  categoryReader->setCategory(categoryToLoad) ;
  categoryReader->setRecordsLimit(30) ;
  connect(categoryReader, SIGNAL(finished()), this, SLOT(processResult()) ) ;

  bool started = categoryReader->start();
  if (!started) {
    errorMessage = categoryReader->getErrorMessage();
    delete categoryReader;
    categoryReader = 0;
    return false;
  }

  return true;
}

// === =======================================================================

void ActionWikiCategoryLoader::processResult() {
  const QString em = categoryReader->getErrorMessage();
  if (!em.isEmpty()) {
    c5w(c5, __c5_MN__, "ERROR: " + em);
    QCoreApplication::exit(1);
    return;
  }

  QStringList ctl = categoryReader->getCategoryTitles();
  categoryTitles.append(ctl);
  c5d(c5, __c5_MN__,
      QString("got %1 elements (%2 total)")
        .arg( ctl.count() ).arg(categoryTitles.count()));
  const QString ncid = categoryReader->getNextContinueId();
  if (ncid.isEmpty()) {
    if (resultFileName.isEmpty()) {
      printResultList();
    }
    else {
      saveResultList();
    }

    delete categoryReader;
    categoryReader = 0;

    emit actionDone();
  }
  else {
    categoryReader->resetCategoryTitles();
    categoryReader->setContinueId(ncid);
    categoryReader->start();
  }
}

// === =======================================================================

void ActionWikiCategoryLoader::printResultList() {
  c5i(c5, "===============================================================");

  c5i(c5, "Finally got this items:");

  QStringList::const_iterator sli = categoryTitles.constBegin();
  for(; sli!=  categoryTitles.constEnd(); sli++) {
    c5i(c5, *sli);
  }

  c5i(c5, QString("Total %1 items").arg(categoryTitles.count()) );
  c5i(c5, "===============================================================");
}

// === =======================================================================

bool ActionWikiCategoryLoader::saveResultList() {
  QFile fl(resultFileName);
  if (fl.exists()) {
    errorMessage = QString("File \"%1\" already exists").arg(resultFileName) ;
    return false;
  }

  bool tboo = fl.open(QIODevice::WriteOnly | QIODevice::Text);
  if (!tboo) {
    errorMessage = "Failed to open file" ;
    return false;
  }

  QByteArray data = categoryTitles.join("\n").toUtf8();
  fl.write( data ) ;
  fl.flush();
  fl.close();

  return true;
}

// === =======================================================================
// === =======================================================================

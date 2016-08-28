#include "TinPigeonApp.h"

// === =======================================================================

#include <QDebug>

#include <QCommandLineParser>
#include <QMap>
//#include <math.h>       // modf
//using namespace std;

// === =======================================================================
// === =======================================================================

TinPigeonApp::TinPigeonApp(QObject *) {
  appExitCode=0;//
}

// === =======================================================================

TinPigeonApp::~TinPigeonApp() {
  //
}

// === =======================================================================

bool TinPigeonApp::doEveryting() {
  bool result = true; //

  return result;
}

// === =======================================================================

bool TinPigeonApp::processCommandLine() {
  bool result = true;
  QMap<QString, QString> options;

  QCommandLineParser parser;
  parser.setApplicationDescription("Test helper");
  parser.addHelpOption();
  parser.addVersionOption();

  // An option with a value
  QCommandLineOption qmlFileNameOption("qml");
  qmlFileNameOption.setDescription("A qml file to display");
  qmlFileNameOption.setValueName("file");
  parser.addOption(qmlFileNameOption);

  // Process the actual command line arguments given by the user
  QCoreApplication* ca = QCoreApplication::instance();
  parser.process(*ca);

  const QString qmlFileName = parser.value(qmlFileNameOption);
  if (!qmlFileName.isEmpty()) {
    options.insert("qml", qmlFileName);
  }
  else {
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

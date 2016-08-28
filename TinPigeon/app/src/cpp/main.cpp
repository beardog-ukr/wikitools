#include <QCoreApplication>

#include <QDebug>

#include "TinPigeonApp.h"

//=============================================================================

// bool parseCommandLineOptions(QMap<QString, QString>& options) {
//   bool result = true;
//
//   QCommandLineParser parser;
//   parser.setApplicationDescription("Test helper");
//   parser.addHelpOption();
//   parser.addVersionOption();
//
//   // An option with a value
//   QCommandLineOption qmlFileNameOption("qml");
//   qmlFileNameOption.setDescription("A qml file to display");
//   qmlFileNameOption.setValueName("file");
//   parser.addOption(qmlFileNameOption);
//
//   // Process the actual command line arguments given by the user
//   QCoreApplication* ca = QCoreApplication::instance();
//   parser.process(*ca);
//
//   const QString qmlFileName = parser.value(qmlFileNameOption);
//   if (!qmlFileName.isEmpty()) {
//     options.insert("qml", qmlFileName);
//   }
//   else {
//     result = false;
//   }
//
//   if (!result) {
//     qDebug() << parser.helpText() ;
//   }
//
//   return result;
// }

//=============================================================================
//=============================================================================
//=============================================================================

int main(int argc, char* argv[])
{
  QCoreApplication app(argc,argv);
  TinPigeonApp tinPigeonApp;

  if (!tinPigeonApp.processCommandLine()) {
    return tinPigeonApp.getAppExitCode();
  }
  //view.connect(view.engine(), &QQmlEngine::quit, &app, &QCoreApplication::quit);

  if (!tinPigeonApp.doEveryting()) {
    return tinPigeonApp.getAppExitCode();
  }

  return app.exec();
}

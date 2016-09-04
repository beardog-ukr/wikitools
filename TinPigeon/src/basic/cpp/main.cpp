#include <QCoreApplication>

#include <QDebug>

#include "TinPigeonApp.h"

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

  if (!tinPigeonApp.prepareToStart()) {
    return tinPigeonApp.getAppExitCode();
  }

  return app.exec();
}

#ifndef __TinPigeonApp__
#define __TinPigeonApp__

#include <QObject>
#include <string>

class ActionBasic;
class FiveCatsLogger;

class QTimer;
class QNetworkAccessManager;

#include <QStringList>

class TinPigeonApp : public QObject
{
  Q_OBJECT

  friend class TestTinPigeonApp;

public:
  TinPigeonApp(QObject *parent = 0);
  virtual ~TinPigeonApp();

  bool prepareToStart();
  bool processCommandLine();

  int getAppExitCode() const;

public slots:
  void processActionResult();
  void startEveryting();

protected:
//  void processResultList();
  ActionBasic* actionPerformer;

  QTimer* startupTimer;

  int appExitCode ;
  FiveCatsLogger* c5;
};

#endif

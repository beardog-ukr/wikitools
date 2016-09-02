#include <QObject>
#include <string>

//class BasicHttpReader;
class SimplePageReader;

class QTimer;
class QNetworkAccessManager;

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
  void downloaderFinished();
  void startEveryting();

protected:
  int appExitCode;

  QString urlToLoad ;

  SimplePageReader* pageReader;

  QTimer* startupTimer;
  QNetworkAccessManager* networkAccessManager;
};

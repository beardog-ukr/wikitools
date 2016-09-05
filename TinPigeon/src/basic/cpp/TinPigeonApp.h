#include <QObject>
#include <string>

//class BasicHttpReader;
//class SimplePageReader;
class WikiCategoryReader;

class FiveCatsLogger;

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
  void processCategories();
  void startEveryting();

protected:
  int appExitCode;

  QString categoryToLoad;
  QString wikiLanguage;

  WikiCategoryReader* categoryReader;

  QTimer* startupTimer;
  QNetworkAccessManager* networkAccessManager;

  FiveCatsLogger* c5;
};

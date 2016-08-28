#include <QObject>
#include <string>

class TinPigeonApp : public QObject
{
  Q_OBJECT

  friend class TestTinPigeonApp;

public:
  TinPigeonApp(QObject *parent = 0);
  virtual ~TinPigeonApp();

  bool doEveryting();
  bool processCommandLine();

  int getAppExitCode() const;

protected:
  int appExitCode;
};

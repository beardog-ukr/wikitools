#ifndef __ActionBasic__
#define __ActionBasic__

class FiveCatsLogger;

#include <QString>
#include <QObject>
class QCommandLineParser;

class ActionBasic: public QObject
{
  Q_OBJECT
  friend class TestBasic;
public:
  ActionBasic(QObject *parent = 0);
  virtual ~ActionBasic();

  virtual bool startAction() =0;

  //static void initCommandLineParser(QCommandLineParser& parser);
  virtual bool loadCommandLineParser(const QCommandLineParser& parser) =0;

  QString getErrorMessage() const;
  void setLogger(FiveCatsLogger* c5);

signals:
  void actionDone();
  void actionFailure();

protected:

//  int appExitCode;

  QString errorMessage;

  FiveCatsLogger* c5;
};



#endif

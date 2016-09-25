#ifndef __ActionLoginPerformer__
#define __ActionLoginPerformer__

class TokenLoader;
class LoginPerformer;
#include "ActionBasic.h"

// === =======================================================================

#include <QStringList>
class QNetworkAccessManager;
class QCommandLineParser;

// === =======================================================================

//
//
//

class ActionLoginPerformer : public ActionBasic {
  Q_OBJECT
  friend class TestBasic;
public:
  ActionLoginPerformer(QObject *parent = 0);
  virtual ~ActionLoginPerformer();

  //
  static void initCommandLineParser(QCommandLineParser& parser);
  virtual bool loadCommandLineParser(const QCommandLineParser& parser);

  virtual bool startAction();

protected slots:
  void processTokenLoaded();
  void processResult();

protected:
//  QString loginToken ;
//  QString tokenType;
  QString wikiLanguage;

  TokenLoader* tokenLoader;
  LoginPerformer* loginPerformer;
  QNetworkAccessManager* networkAccessManager;
};

#endif

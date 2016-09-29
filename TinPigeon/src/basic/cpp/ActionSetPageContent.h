#ifndef __ActionSetPageContent__
#define __ActionSetPageContent__

class EditingPerformer ;
class TokenLoader;
class LoginPerformer;
class EditingTokenLoader;
#include "ActionBasic.h"

// === =======================================================================

#include <QStringList>
class QNetworkAccessManager;
class QCommandLineParser;

// === =======================================================================

//  Sets some page content (reads content from  file)
//  Makes all following steps: gets login token, logs in, gets edit token,
// performs edit
//

class ActionSetPageContent : public ActionBasic {
  Q_OBJECT
  friend class TestBasic;
public:
  ActionSetPageContent(QObject *parent = 0);
  virtual ~ActionSetPageContent();

  //
  static void initCommandLineParser(QCommandLineParser& parser);
  virtual bool loadCommandLineParser(const QCommandLineParser& parser);

  virtual bool startAction();

protected slots:
  void processTokenLoaded() ;
  void processLoggedIn() ;
  void processEditTokenReceived() ;
  void processPageEditComplete() ;

protected:
  QString loginToken ;
  QString editingToken;
  QString pageTimestamp ;

  QString wikiLanguage;
  QString pageTitle ;
  QString pageText;
  QString login;
  QString password;
  QString editSummary;

  EditingPerformer* editor ;
  EditingTokenLoader* editingTokenLoader;
  LoginPerformer* loginPerformer;
  TokenLoader* tokenLoader;
  QNetworkAccessManager* networkAccessManager;

  bool readCredentials(const QString& fn) ;
  bool readNewText(const QString& fn) ;
};

#endif

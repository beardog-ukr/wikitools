#ifndef __ActionReplaceEditor__
#define __ActionReplaceEditor__

class ArticleWikitextReader ;
class TokenLoader;
class LoginPerformer;
class EditingTokenLoader;
#include "ActionBasic.h"

// === =======================================================================

#include <QStringList>
class QNetworkAccessManager;
class QCommandLineParser;

// === =======================================================================

//
//
//

class ActionReplaceEditor : public ActionBasic {
  Q_OBJECT
  friend class TestBasic;
public:
  ActionReplaceEditor(QObject *parent = 0);
  virtual ~ActionReplaceEditor();

  //
  static void initCommandLineParser(QCommandLineParser& parser);
  virtual bool loadCommandLineParser(const QCommandLineParser& parser);

  virtual bool startAction();

protected slots:
  void processTokenLoaded() ;
  void processLoggedIn() ;
  void processWikitextLoaded() ;

protected:
  QString loginToken ;
  QString editingToken;

  QString wikiLanguage;
  QString pageTitle ;
  QString inStr;
  QString outStr;

  ArticleWikitextReader* wikitextLoader ;
  EditingTokenLoader* editingTokenLoader;
  LoginPerformer* loginPerformer;
  TokenLoader* tokenLoader;
  QNetworkAccessManager* networkAccessManager;
};

#endif

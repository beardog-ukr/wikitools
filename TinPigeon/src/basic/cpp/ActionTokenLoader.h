#ifndef __ActionTokenLoader__
#define __ActionTokenLoader__

class TokenLoader;
#include "ActionBasic.h"

// === =======================================================================

#include <QStringList>
class QNetworkAccessManager;
class QCommandLineParser;

// === =======================================================================

// This is a request for tokens; received token later can be used to perform some actions
// See
//  https://www.mediawiki.org/wiki/API:Tokens
//  https://www.mediawiki.org/w/api.php?action=help&modules=query%2Btokens

class ActionTokenLoader : public ActionBasic {
  Q_OBJECT
  friend class TestBasic;
public:
  ActionTokenLoader(QObject *parent = 0);
  virtual ~ActionTokenLoader();

  // "--lang" and "--type"
  static void initCommandLineParser(QCommandLineParser& parser);
  virtual bool loadCommandLineParser(const QCommandLineParser& parser);

  virtual bool startAction();

protected slots:
  void processResult();

protected:
  QString token ;
  QString tokenType;
  QString wikiLanguage;

  TokenLoader* tokenLoader;
  QNetworkAccessManager* networkAccessManager;
};

#endif

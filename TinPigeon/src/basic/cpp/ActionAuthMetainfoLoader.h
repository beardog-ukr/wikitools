#ifndef __ActionAuthMetainfoLoader__
#define __ActionAuthMetainfoLoader__

class AuthMetainfoLoader;
#include "ActionBasic.h"

// === =======================================================================

#include <QStringList>
class QNetworkAccessManager;
class QCommandLineParser;

// === =======================================================================

// This action receives info about login fields required for some wiki
// See
//  https://www.mediawiki.org/wiki/API:Authmanagerinfo
//  https://www.mediawiki.org/wiki/API:Login#The_clientlogin_action
// also
//  https://www.mediawiki.org/wiki/API:Tokens
//  https://www.mediawiki.org/w/api.php?action=help&modules=query%2Btokens

class ActionAuthMetainfoLoader : public ActionBasic {
  Q_OBJECT
  friend class TestBasic;
public:
  ActionAuthMetainfoLoader(QObject *parent = 0);
  virtual ~ActionAuthMetainfoLoader();

  // only one command line option: "--lang"
  static void initCommandLineParser(QCommandLineParser& parser);
  virtual bool loadCommandLineParser(const QCommandLineParser& parser);

  virtual bool startAction();

protected slots:
  void processResult();

protected:
  QString wikiLanguage ;

  AuthMetainfoLoader* pageReader;
  QNetworkAccessManager* networkAccessManager;

};

#endif

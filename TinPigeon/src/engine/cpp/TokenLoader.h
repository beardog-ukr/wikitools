#ifndef __TokenLoader__
#define __TokenLoader__

#include "BasicHttpReader.h"

#include <QStringList>
#include <QMap>
class QJsonObject ;

// === =======================================================================

class TokenLoader : public BasicHttpReader
{
  Q_OBJECT
  friend class TestEngine;
public:
  TokenLoader(QNetworkAccessManager* nam, QObject* parent =0);
  virtual ~TokenLoader();

  QString getToken(const QString& tt) const;

  void setWiki(const QString& langCode) ;
  void addTokenType(const QString& tt) ;

protected :
  TokenLoader();
  void setupAll() ;

  virtual QNetworkReply* makeRequest();
  virtual bool processReceivedData();

  bool readValuesFromJson(QJsonObject*);

  QString langCode;
  QStringList tokensToLoad;
  QMap<QString, QString> values;
};

// === =======================================================================

#endif

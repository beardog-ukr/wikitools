#ifndef __LoginPerformer__
#define __LoginPerformer__

#include "BasicHttpReader.h"

#include <QStringList>
#include <QVariant>
#include <QMap>
class QJsonObject ;

// === =======================================================================

class LoginPerformer : public BasicHttpReader
{
  Q_OBJECT
  friend class TestEngine;
public:
  LoginPerformer(QNetworkAccessManager* nam, QObject* parent =0);
  virtual ~LoginPerformer();

  QString getToken(const QString& tt) const;

  void setWiki(const QString& langCode) ;
  void setLoginToken(const QString& lt) ;
  void setUserName(const QString& un) ;
  void setUserPassword(const QString& up) ;
  void setCookieHeader(const QVariant ch) ;

  QString getAllResponse() const;

protected :
  LoginPerformer();
  void setupAll() ;

  virtual QNetworkReply* makeRequest();
  virtual bool processReceivedData();

  bool readValuesFromJson(QJsonObject*);

  QString langCode;
  QString loginToken;
  QString userName;
  QString userPassword ;
  QVariant cookieHeader ;

  QString allResponse;
};

// === =======================================================================

#endif

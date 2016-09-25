#ifndef __AuthMetainfoLoader__
#define __AuthMetainfoLoader__

#include "BasicHttpReader.h"

#include <QStringList>

// === =======================================================================

class AuthMetainfoLoader : public BasicHttpReader
{
  Q_OBJECT
  friend class TestEngine;
public:
  AuthMetainfoLoader(QNetworkAccessManager* nam, QObject* parent =0);
  virtual ~AuthMetainfoLoader();

  QString getAllResponse() const;

  void setWiki(const QString& langCode);

protected :
  AuthMetainfoLoader();
  void setupAll() ;

  virtual QNetworkReply* makeRequest();
  virtual bool processReceivedData();

  QString langCode;
  QString allResponse;
};

// === =======================================================================

#endif

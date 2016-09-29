#ifndef __EditingTokenLoader__
#define __EditingTokenLoader__

#include "BasicHttpReader.h"
// === =======================================================================
#include <QStringList>
class QJsonDocument;

// === =======================================================================

class EditingTokenLoader : public BasicHttpReader
{
  Q_OBJECT

  friend class TestEngine;

public:
  EditingTokenLoader(QNetworkAccessManager* nam, QObject* parent =0);
  virtual ~EditingTokenLoader();

  void setWiki(const QString& langCode) ;
  void setPageTitle(const QString& pt) ;
  QString getToken() const;
  QString getTimestamp() const;

protected :
  EditingTokenLoader();

  void setupAll();

  virtual QNetworkReply* makeRequest();
  virtual bool processReceivedData();

  bool readValuesFromJson(QJsonObject* obj) ;

  QString langCode;
  QString title;
  QString timestamp;
  QString token;
};

// === =======================================================================

#endif

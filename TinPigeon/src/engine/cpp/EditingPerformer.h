#ifndef __EditingPerformer__
#define __EditingPerformer__

#include "BasicHttpReader.h"

#include <QStringList>
#include <QVariant>
#include <QMap>
class QJsonObject ;

// === =======================================================================

class EditingPerformer : public BasicHttpReader
{
  Q_OBJECT
  friend class TestEngine;
public:
  EditingPerformer(QNetworkAccessManager* nam, QObject* parent =0);
  virtual ~EditingPerformer();

  QString getToken(const QString& tt) const;

  void setEditToken(const QString& et) ;
  void setNewtext(const QString& nt) ;
  void setTimestamp(const QString& ts) ;
  void setPageTitle(const QString& up) ;
  void setSummary(const QString& ss) ;
  void setWiki(const QString& langCode) ;

protected :
  EditingPerformer();
  void setupAll() ;

  virtual QNetworkReply* makeRequest();
  virtual bool processReceivedData();

  bool readValuesFromJson(QJsonObject*);

  QString langCode;
  QString editToken;
  QString pageTitle;
  QString timestamp ;
  QString newText ;
  QString summary ;
};

// === =======================================================================

#endif

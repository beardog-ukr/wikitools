#include "EditingPerformer.h"
#include "FiveCatsLogger.h"
#include "ErrorResponseProcessor.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QUrl>
#include <QUrlQuery>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QVariant>

// === =======================================================================
// === =======================================================================

EditingPerformer::EditingPerformer()
                 :BasicHttpReader(0,0) {
  setupAll();
}

// === =======================================================================

EditingPerformer::EditingPerformer(QNetworkAccessManager* nam, QObject* parent)
               :BasicHttpReader(nam,parent) {
  setupAll();
}

// === =======================================================================

EditingPerformer::~EditingPerformer() {
//ntdh
}

// === =======================================================================

void EditingPerformer::setupAll() {
  langCode = "test";
  editToken = "" ;
  timestamp = "";
  pageTitle = "" ;
  newText = "" ;
}

// === =======================================================================

void EditingPerformer::setWiki(const QString& lc) {
  langCode = lc ;
}

// === =======================================================================

void EditingPerformer::setEditToken(const QString& et) {
  editToken = et;
}

// === =======================================================================

void EditingPerformer::setTimestamp(const QString& ts) {
  timestamp = ts ;
}

// === =======================================================================

void EditingPerformer::setPageTitle(const QString& pt) {
  pageTitle = pt ;
}

// === =======================================================================

void EditingPerformer::setNewtext(const QString& nt) {
  newText = nt ;
}

// === =======================================================================

void EditingPerformer::setSummary(const QString& ss) {
  summary = ss ;
}

// === =======================================================================

QNetworkReply* EditingPerformer::makeRequest() {
  const QString endpointTemplate = "https://%1.wikipedia.org/w/api.php" ;
  const QString endpoint = endpointTemplate.arg(langCode);
  c5t(c5, __c5_MN__, "Endpoint is " + endpoint);

  QUrl url(endpoint);

  QUrlQuery query ;
  query.addQueryItem("action", "edit");
  query.addQueryItem("format", "json");
  query.addQueryItem("title", pageTitle);
  query.addQueryItem("summary", summary );
  query.addQueryItem("basetimestamp", timestamp);
  query.addQueryItem("text", newText );
  query.addQueryItem("token", editToken ); // token should go after text
  query.addQueryItem("bot", "1" );


  QString qs = query.query(QUrl::FullyEncoded) ;
  qs.replace("+", "%2B");
  QByteArray qsba = qs.toUtf8();
  c5t(c5, "QS is " + qs);

  QNetworkRequest nrq(url);
  nrq.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
  nrq.setHeader(QNetworkRequest::ContentLengthHeader, qsba.count());

  return networkAccessManager->post( nrq, qsba);
}

// === =======================================================================

bool EditingPerformer::processReceivedData() {
  //c5d(c5, __c5_MN__,  QString::fromUtf8(receivedData));



  QJsonParseError jperr;
  QJsonDocument doc = QJsonDocument::fromJson(receivedData, &jperr);
  if (jperr.error!=QJsonParseError::NoError) {
    errorMessage = "JSON ERROR: " + jperr.errorString();
    c5d(c5, __c5_MN__, errorMessage);
    return false;
  }

  QJsonObject obj = doc.object();
  if (obj.isEmpty()) {
    errorMessage = "Failed to get expected json object";
    return false;
  }

  ErrorResponseProcessor erp;
  erp.loadJson(&obj);
  if (erp.hasError()) {
    errorMessage = QString("Received error: %2 (%1)")
                      .arg(erp.errorCode(), erp.errorInfo());
    return false;
  }

  bool result = true;
//  result = readValuesFromJson(&obj);
//  if (!result) {
//    c5d(c5, "Error with this context:\n" + QString::fromUtf8(receivedData) + "\n");
//  }

  return result;
}

// === =======================================================================

bool EditingPerformer::readValuesFromJson(QJsonObject* obj) {
//  if (!obj->contains("login")) {
//    errorMessage = "Failed to get expected \"login\" object";
//    return false;
//  }

//  QJsonValue lobjValue = obj->value("login");
//  if (!lobjValue.isObject()) {
//    errorMessage = "Failed to get expected \"login\" as object";
//    return false;
//  }
//  QJsonObject lobj = lobjValue.toObject();

//  QString resultString = lobj.value("result").toString();
//  if (resultString != "Success") {
//    errorMessage = "Incorrect login\\result value";
//    return false;
//  }

//  lgToken = lobj.value("lgtoken").toString();
//  if (lgToken.isEmpty()) {
//    errorMessage = "Incorrect login\\lgtoken value";
//    return false;
//  }

  return true;
}

// === =======================================================================
// === =======================================================================

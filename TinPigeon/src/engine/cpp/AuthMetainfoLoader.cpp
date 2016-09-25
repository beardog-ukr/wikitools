#include "AuthMetainfoLoader.h"
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

AuthMetainfoLoader::AuthMetainfoLoader()
                   :BasicHttpReader(0,0) {
  setupAll();
}

// === =======================================================================

AuthMetainfoLoader::AuthMetainfoLoader(QNetworkAccessManager* nam, QObject* parent)
                   :BasicHttpReader(nam,parent) {
  setupAll();
}

// === =======================================================================

AuthMetainfoLoader::~AuthMetainfoLoader() {
//ntdh
}

// === =======================================================================

void AuthMetainfoLoader::setupAll() {
  allResponse = "";
  langCode = "en";
}

// === =======================================================================

void AuthMetainfoLoader::setWiki(const QString& lc) {
  langCode = lc ;
}

// === =======================================================================

QString AuthMetainfoLoader::getAllResponse() const{
  return allResponse ;
}

// === =======================================================================

QNetworkReply* AuthMetainfoLoader::makeRequest() {
  const QString endpointTemplate = "https://%1.wikipedia.org/w/api.php" ;
  const QString endpoint = endpointTemplate.arg(langCode);
  c5t(c5, __c5_MN__, "Endpoint is " + endpoint);

  QUrl url(endpoint);

  QUrlQuery query ;
  query.addQueryItem("action", "query");
  query.addQueryItem("format", "json");
  query.addQueryItem("meta", "authmanagerinfo");
  query.addQueryItem("amirequestsfor","login");

  url.setQuery(query);

  return networkAccessManager->get(QNetworkRequest(url));
}

// === =======================================================================

bool AuthMetainfoLoader::processReceivedData() {
//  c5d(c5, __c5_MN__,  QString::fromUtf8(receivedData));

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


  //finally
  allResponse = QString::fromUtf8(receivedData);

//  bool result = readWikitextFromJson(&obj);
//  if (!result) {
//    c5d(c5, "Error with this context:\n" + QString::fromUtf8(receivedData) + "\n");
//  }

  bool result = true;
  return result;
}

// === =======================================================================
// === =======================================================================

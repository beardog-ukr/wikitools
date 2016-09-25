#include "TokenLoader.h"
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

TokenLoader::TokenLoader()
            :BasicHttpReader(0,0) {
  setupAll();
}

// === =======================================================================

TokenLoader::TokenLoader(QNetworkAccessManager* nam, QObject* parent)
            :BasicHttpReader(nam,parent) {
  setupAll();
}

// === =======================================================================

TokenLoader::~TokenLoader() {
//ntdh
}

// === =======================================================================

void TokenLoader::setupAll() {
  langCode = "en";
}

// === =======================================================================

void TokenLoader::setWiki(const QString& lc) {
  langCode = lc ;
}

// === =======================================================================

QString TokenLoader::getToken(const QString& tt) const {
  return values.value(tt);
}

// === =======================================================================

void TokenLoader::addTokenType(const QString& tt) {
  if ( (tt!="csrf") && (tt!= "watch") && (tt!= "patrol") &&
       (tt!="rollback") && (tt!="userrights") &&
       (tt!="login") && (tt!="createaccount") ) {
    errorMessage = "Unknown token type " + tt;
    return;
  }

  tokensToLoad.append(tt);
  return ;
}

// === =======================================================================

QNetworkReply* TokenLoader::makeRequest() {
  const QString endpointTemplate = "https://%1.wikipedia.org/w/api.php" ;
  const QString endpoint = endpointTemplate.arg(langCode);
  c5t(c5, __c5_MN__, "Endpoint is " + endpoint);

  QUrl url(endpoint);

  QUrlQuery query ;
  query.addQueryItem("action", "query");
  query.addQueryItem("format", "json");
  query.addQueryItem("meta", "tokens");
  query.addQueryItem("type", tokensToLoad.join("|"));

  url.setQuery(query);

  return networkAccessManager->get(QNetworkRequest(url));
}

// === =======================================================================

bool TokenLoader::processReceivedData() {
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

  bool result = readValuesFromJson(&obj);
  if (!result) {
    c5d(c5, "Error with this context:\n" + QString::fromUtf8(receivedData) + "\n");
  }

  return result;
}

// === =======================================================================

bool TokenLoader::readValuesFromJson(QJsonObject* obj) {
  values.clear();

  if (!obj->contains("query")) {
    errorMessage = "Failed to get expected \"query\" object";
    return false;
  }

  QJsonValue qobjValue = obj->value("query");
  if (!qobjValue.isObject()) {
    errorMessage = "Failed to get expected \"query\" as object";
    return false;
  }

  QJsonObject qobj = qobjValue.toObject();


  QJsonValue tobjValue = qobj.value("tokens");
  if (!tobjValue.isObject()) {
    errorMessage = "Failed to get expected \"tokens\" as object";
    return false;
  }

  QJsonObject tobj = tobjValue.toObject();

  QStringList::const_iterator ttli = tokensToLoad.constBegin();
  for (;ttli!=tokensToLoad.constEnd(); ttli++) {
    QString tStr= *ttli + "token";
    QJsonValue pairValue = tobj.value(tStr);
    if (pairValue.isString()) {
      values[*ttli] = pairValue.toString();
    }
    else {
      errorMessage = QString("Failed to find %1 value").arg(*ttli) ;
      return false;
    }
  }

  return true;
}

// === =======================================================================
// === =======================================================================

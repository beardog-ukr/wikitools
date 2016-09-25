#include "WikiCategoryReader.h"
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

WikiCategoryReader::WikiCategoryReader()
                   :BasicHttpReader(0,0) {
  setupAll();
}

// === =======================================================================

WikiCategoryReader::WikiCategoryReader(QNetworkAccessManager* nam, QObject* parent)
               :BasicHttpReader(nam,parent) {
  setupAll();
}

// === =======================================================================

void WikiCategoryReader::setupAll() {
  category = "";
  langCode = "en";
  recordsLimit = -1;
  currentContinueId = "";
  nextContinueId = "";
}

// === =======================================================================

bool WikiCategoryReader::setContinueId(const QString& cid) {
  currentContinueId = cid;
  return true;
}

// === =======================================================================

QString WikiCategoryReader::getNextContinueId() const {
  return nextContinueId;
}

// === =======================================================================

WikiCategoryReader::~WikiCategoryReader() {
// nothing to do here
}

// === =======================================================================

bool WikiCategoryReader::setWiki(const QString& langCodeStr) {
  langCode = langCodeStr;
  return true;
}

// === =======================================================================

bool WikiCategoryReader::setCategory(const QString& categoryStr) {
  category = categoryStr;
  return true;
}

// === =======================================================================

QStringList WikiCategoryReader::getCategoryTitles() const {
  return titles;
}

// === =======================================================================

QNetworkReply* WikiCategoryReader::makeRequest() {
  const QString endpointTemplate = "https://%1.wikipedia.org/w/api.php" ;
  const QString endpoint = endpointTemplate.arg(langCode);
  c5t(c5, __c5_MN__, "Endpoint is " + endpoint);

  QUrl url(endpoint);

  QUrlQuery query ;
  query.addQueryItem("action", "query");
  query.addQueryItem("format", "json");
  query.addQueryItem("uselang", "user");
  query.addQueryItem("list","categorymembers");
  query.addQueryItem("cmtitle", category);
  if (!currentContinueId.isEmpty()) {
    query.addQueryItem("cmcontinue", currentContinueId);
  }
  if (recordsLimit>0) {
    query.addQueryItem("cmlimit", QString::number(recordsLimit));
  }

  url.setQuery(query);

  return networkAccessManager->get(QNetworkRequest(url));
}

// === =======================================================================

bool WikiCategoryReader::processReceivedData() {
  c5d(c5, __c5_MN__,  QString::fromUtf8(receivedData));

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

  if (!readContinueId(&obj)) {
    return false;
  }

  return readElementsFromJsonDoc(&obj, titles );
}

// === =======================================================================

bool WikiCategoryReader::readElementsFromJsonDoc(QJsonObject* obj, QStringList& elements) {
  elements.clear();

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

  QJsonValue cmarrValue = qobj.value("categorymembers");
  if (cmarrValue.type()!=QJsonValue::Array) {
    errorMessage = "Failed to get expected \"query\\categorymembers\"";
  }

  QJsonArray cmarr = cmarrValue.toArray();
  if (cmarr.isEmpty()) {
    errorMessage = "Received empty categorymembers array";
    return false;
  }
  QJsonArray::const_iterator cmi;
  for (cmi = cmarr.constBegin(); cmi != cmarr.constEnd(); ++cmi) {
    QJsonObject cmo = cmi->toObject();
    QJsonValue tjv;
    tjv = cmo.value("title");
    if (tjv.type()!=QJsonValue::String) {
      errorMessage = "Received bad \"title\" element in \"query\\categorymembers\"";
      return false;
    }
    else {
      elements.append(tjv.toString());
    }
  }

  return true;
}

// === =======================================================================

bool WikiCategoryReader::readContinueId(QJsonObject* jsonObj) {
  nextContinueId = "";

  if (!jsonObj->contains("continue")) {
    return true;
  }

  QJsonValue cv = jsonObj->value("continue");
  if (!cv.isObject()) {
    errorMessage = "Unexpected continue content (not an object)";
    return false;
  }

  QJsonObject cvobj = cv.toObject();

  if (cvobj.contains("cmcontinue")) {
    nextContinueId = cvobj.value("cmcontinue").toString();
  }
  else {
    errorMessage = "Unexpected continue content (no cmcontinue)";
    return false;
  }

  return true;
}

void WikiCategoryReader::resetCategoryTitles() {
  titles.clear();
}

// === =======================================================================

bool WikiCategoryReader::setRecordsLimit(const int limit) {
  if (limit<=0) {
    return false ;
  }

  recordsLimit = limit;

  return true;
}

// === =======================================================================
// === =======================================================================

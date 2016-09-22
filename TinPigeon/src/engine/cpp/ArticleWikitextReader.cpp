#include "ArticleWikitextReader.h"
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

ArticleWikitextReader::ArticleWikitextReader()
                      :BasicHttpReader(0,0) {
  setupAll();
}

// === =======================================================================

ArticleWikitextReader::ArticleWikitextReader(QNetworkAccessManager* nam, QObject* parent)
               :BasicHttpReader(nam,parent) {
  setupAll();
}

// === =======================================================================

ArticleWikitextReader::~ArticleWikitextReader() {
//ntdh
}

// === =======================================================================

void ArticleWikitextReader::setupAll() {
  wikitext = "";
  langCode = "en";
  pageTitle = "";
}

// === =======================================================================

void ArticleWikitextReader::setWiki(const QString& lc) {
  langCode = lc ;
}

// === =======================================================================

void ArticleWikitextReader::setPageTitle(const QString& pt) {
  pageTitle = pt ;
}

// === =======================================================================

QString ArticleWikitextReader::getPageTitle() const {
  return pageTitle;
}

// === =======================================================================

void ArticleWikitextReader::resetWikitext() {
  wikitext = "" ;
}

// === =======================================================================

QString ArticleWikitextReader::getWikitext() const{
  return wikitext ;
}

// === =======================================================================

QNetworkReply* ArticleWikitextReader::makeRequest() {
  const QString endpointTemplate = "https://%1.wikipedia.org/w/api.php" ;
  const QString endpoint = endpointTemplate.arg(langCode);
  c5t(c5, __c5_MN__, "Endpoint is " + endpoint);

  QUrl url(endpoint);

  QUrlQuery query ;
  query.addQueryItem("action", "parse");
  query.addQueryItem("format", "json");
  query.addQueryItem("page", pageTitle);
  query.addQueryItem("prop","wikitext");

  url.setQuery(query);

  return networkAccessManager->get(QNetworkRequest(url));
}

// === =======================================================================

bool ArticleWikitextReader::processReceivedData() {
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


  bool result = readWikitextFromJson(&obj);
  if (!result) {
    c5d(c5, "Error with this context:\n" + QString::fromUtf8(receivedData) + "\n");
  }

  return result;
}

// === =======================================================================

bool ArticleWikitextReader::readWikitextFromJson(QJsonObject* rootObj) {
  wikitext = "";

  if (!rootObj->contains("parse")) {
    errorMessage = "Failed to get expected \"parse\" object";
    return false;
  }
  QJsonValue pov = rootObj->value("parse");
  if (!pov.isObject()) {
    errorMessage = "Failed to get expected \"parse\" as object";
    return false;
  }
  QJsonObject po = pov.toObject();

  if (!po.contains("wikitext")) {
    errorMessage = "Failed to get expected \"wikitext\" object";
    return false;
  }
  QJsonValue wtv = po.value("wikitext");
  if (!wtv.isObject()) {
    errorMessage = "Failed to get expected \"wikitext\" as object";
    return false;
  }
  QJsonObject wto = wtv.toObject();

  QJsonValue wtav = wto.value("*");
  if (wtav.type()!=QJsonValue::String) {
    errorMessage = "Received bad \"*\" element in \"wt\\parse\"";
    return false;
  }

  wikitext = wtav.toString();
  return true;
}

// === =======================================================================
// === =======================================================================

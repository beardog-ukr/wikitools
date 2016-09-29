#include "EditingTokenLoader.h"
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
#include <QRegularExpression>

// === =======================================================================
// === =======================================================================

EditingTokenLoader::EditingTokenLoader()
                   :BasicHttpReader(0,0) {
  setupAll();
}

// === =======================================================================

EditingTokenLoader::EditingTokenLoader(QNetworkAccessManager* nam, QObject* parent)
                   :BasicHttpReader(nam,parent) {
  setupAll();
}

// === =======================================================================

void EditingTokenLoader::setupAll() {
  langCode = "test" ;
  title = "";

  token= "" ;
  timestamp = "" ;
}

// === =======================================================================

EditingTokenLoader::~EditingTokenLoader() {
// nothing to do here
}

// === =======================================================================

void EditingTokenLoader::setWiki(const QString& lc) {
  langCode = lc ;
}

// === =======================================================================

void EditingTokenLoader::setPageTitle(const QString& pt) {
  title = pt;
}

// === =======================================================================

QString EditingTokenLoader::getToken() const {
  return token ;
}

// === =======================================================================

QString EditingTokenLoader::getTimestamp() const {
  return timestamp ;
}

// === =======================================================================

QNetworkReply* EditingTokenLoader::makeRequest() {
  const QString endpointTemplate = "https://%1.wikipedia.org/w/api.php" ;
  const QString endpoint = endpointTemplate.arg(langCode);
  c5t(c5, __c5_MN__, "Endpoint is " + endpoint);

  QUrl url(endpoint);

  //n=query&
  //    format=json&
  //prop=info%7Crevisions&
  //titles=Main+Page&
  //intoken=edit&
  //rvprop=timestamp

  QUrlQuery query ;
  query.addQueryItem("action", "query");
  query.addQueryItem("format", "json");
  query.addQueryItem("prop", "info|revisions");
  query.addQueryItem("intoken", "edit");
  query.addQueryItem("rvprop", "timestamp");
  query.addQueryItem("titles", title);

  url.setQuery(query);

  return networkAccessManager->get(QNetworkRequest(url));
}

// === =======================================================================

bool EditingTokenLoader::processReceivedData() {
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

bool EditingTokenLoader::readValuesFromJson(QJsonObject* obj) {
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


  QJsonValue pobjValue = qobj.value("pages");
  if (!pobjValue.isObject()) {
    errorMessage = "Failed to get expected \"pages\" as object";
    return false;
  }

  QJsonObject pobj = pobjValue.toObject();

  QRegularExpression re("^\\d+$");
  QStringList keys= pobj.keys();
  QString pageId ="";
  for(QStringList::const_iterator ki=keys.constBegin();
      ki!= keys.constEnd(); ki++) {
    if (re.match(*ki).hasMatch()) {
      pageId = *ki ;
      break;
    }
  }

  if (pageId.isEmpty()) {
    errorMessage = "Failed to find page id";
    return false;
  }

  QJsonValue nobjValue = pobj.value(pageId);
  if (!nobjValue.isObject()) {
    errorMessage = QString("Failed to get expected specific page(%1) object as object").arg(pageId);
    return false;
  }

  QJsonObject nobj = nobjValue.toObject();

  token = nobj.value("edittoken").toString();
  if (token.isEmpty()) {
    errorMessage = "Failed to read editoken value";
    return false;
  }

  QJsonArray revArr = nobj.value("revisions").toArray();
  if (revArr.count()==0) {
    errorMessage = "Revisions array is empty";
    return false;
  }
  timestamp = revArr.at(0).toObject().value("timestamp").toString();

  if (timestamp.isEmpty()) {
    errorMessage = "Failed to read timestamp value";
    return false;
  }

  return true;
}

// === =======================================================================
// === =======================================================================

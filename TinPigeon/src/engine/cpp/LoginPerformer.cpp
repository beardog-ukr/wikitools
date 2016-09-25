#include "LoginPerformer.h"
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

LoginPerformer::LoginPerformer()
               :BasicHttpReader(0,0) {
  setupAll();
}

// === =======================================================================

LoginPerformer::LoginPerformer(QNetworkAccessManager* nam, QObject* parent)
               :BasicHttpReader(nam,parent) {
  setupAll();
}

// === =======================================================================

LoginPerformer::~LoginPerformer() {
//ntdh
}

// === =======================================================================

void LoginPerformer::setupAll() {
  langCode = "test";
  loginToken = "" ;
  userName = "";
  userPassword = "" ;
}

// === =======================================================================

void LoginPerformer::setWiki(const QString& lc) {
  langCode = lc ;
}

// === =======================================================================

void LoginPerformer::setLoginToken(const QString& lt) {
  loginToken = lt;
}

// === =======================================================================

void LoginPerformer::setUserName(const QString& un) {
  userName = un ;
}

// === =======================================================================

void LoginPerformer::setUserPassword(const QString& up) {
  userPassword = up ;
}

// === =======================================================================

void LoginPerformer::setCookieHeader(const QVariant ch) {
  cookieHeader = ch ;
}


// === =======================================================================

QString LoginPerformer::getAllResponse() const {
  return allResponse ;
}

// === =======================================================================

QNetworkReply* LoginPerformer::makeRequest() {
  const QString endpointTemplate = "https://%1.wikipedia.org/w/api.php" ;
  const QString endpoint = endpointTemplate.arg(langCode);
  c5t(c5, __c5_MN__, "Endpoint is " + endpoint);

  QUrl url(endpoint);

  //api.php?
  //action=clientlogin&
  //loginreturnurl=http://example.com/&
  //logintoken=29590a3037d325be70b93fb8258ed29257448cfb%2B%5C&
  //username=Bob&
  //password=secret&
  //rememberMe=1

  QUrlQuery query ;
//  query.addQueryItem("action", "clientlogin");
//  query.addQueryItem("format", "json");
//  query.addQueryItem("loginreturnurl", "http://example.com/");
//  query.addQueryItem("logintoken", loginToken);
//  query.addQueryItem("username", userName );
//  query.addQueryItem("password", userPassword);

  query.addQueryItem("action", "login");
  query.addQueryItem("format", "json");
  query.addQueryItem("lgtoken", loginToken);
  query.addQueryItem("lgname", userName );
  query.addQueryItem("lgpassword", userPassword);

  QString qs = query.query(QUrl::FullyEncoded) ;
  qs.replace("+", "%2B");
  c5t(c5, "QS is " + qs);
  QByteArray qsba = qs.toUtf8();

  QNetworkRequest nrq(url);
  nrq.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
  nrq.setHeader(QNetworkRequest::ContentLengthHeader, qsba.count());

  return networkAccessManager->post( nrq, qsba);
}

// === =======================================================================

bool LoginPerformer::processReceivedData() {
  //c5d(c5, __c5_MN__,  QString::fromUtf8(receivedData));

  allResponse = QString::fromUtf8(receivedData) ;

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

  bool result = true ;
//  result = readValuesFromJson(&obj);
//  if (!result) {
//    c5d(c5, "Error with this context:\n" + QString::fromUtf8(receivedData) + "\n");
//  }

  return result;
}

// === =======================================================================

//bool TokenLoader::readValuesFromJson(QJsonObject* obj) {
//  values.clear();

//  if (!obj->contains("query")) {
//    errorMessage = "Failed to get expected \"query\" object";
//    return false;
//  }

//  QJsonValue qobjValue = obj->value("query");
//  if (!qobjValue.isObject()) {
//    errorMessage = "Failed to get expected \"query\" as object";
//    return false;
//  }

//  QJsonObject qobj = qobjValue.toObject();


//  QJsonValue tobjValue = qobj.value("tokens");
//  if (!tobjValue.isObject()) {
//    errorMessage = "Failed to get expected \"tokens\" as object";
//    return false;
//  }

//  QJsonObject tobj = tobjValue.toObject();

//  QStringList::const_iterator ttli = tokensToLoad.constBegin();
//  for (;ttli!=tokensToLoad.constEnd(); ttli++) {
//    QString tStr= *ttli + "token";
//    QJsonValue pairValue = tobj.value(tStr);
//    if (pairValue.isString()) {
//      values[*ttli] = pairValue.toString();
//    }
//    else {
//      errorMessage = QString("Failed to find %1 value").arg(*ttli) ;
//      return false;
//    }
//  }

//  return true;
//}

// === =======================================================================
// === =======================================================================

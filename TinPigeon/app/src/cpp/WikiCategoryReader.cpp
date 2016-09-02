#include "WikiCategoryReader.h"

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

#include <QDebug>

// === =======================================================================
// === =======================================================================

WikiCategoryReader::WikiCategoryReader(QNetworkAccessManager* nam, QObject* parent)
               :BasicHttpReader(nam,parent) {
  category = "";
  langCode = "en";
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

QList<WikiCategoryElement> WikiCategoryReader::getCategoryElements() const {
  return elements;
}

// === =======================================================================

QNetworkReply* WikiCategoryReader::makeRequest() {
  const QString endpointTemplate = "https://%1.wikipedia.org/w/api.php" ;
  const QString endpoint = endpointTemplate.arg(langCode);

  QUrl url(endpoint);

  QUrlQuery query ;
  // this is an example:
  //  /w/api.php?action=query&format=json&uselang=user&list=categorymembers&
  //     cmtitle=Category%3ARecipients+of+the+Order+For+Courage%2C+3rd+class

  query.addQueryItem("action", "query");
  query.addQueryItem("format", "json");
  query.addQueryItem("uselang", "user");
  query.addQueryItem("list","categorymembers");
  query.addQueryItem("cmtitle", category);

  url.setQuery(query);

  return networkAccessManager->get(QNetworkRequest(url));
}

// === =======================================================================

bool WikiCategoryReader::processReceivedData() {
  bool result = true;

  // QString tmps = QString::fromUtf8(receivedData);
  // qDebug() << "got :";
  // qDebug() << tmps;

  QJsonParseError jperr;
  QJsonDocument doc = QJsonDocument::fromJson(receivedData, &jperr);
  if (jperr.error!=QJsonParseError::NoError) {
    errorMessage = jperr.errorString();
    return false;
  }

  QJsonObject obj = doc.object();
  if (obj.isEmpty()) {
    errorMessage = "Failed to get expected json object";
    return false;
  }

  if (obj.contains("error")) {
    QJsonValue ev = obj.value("error");
    if (!ev.isObject()) {
      errorMessage = "Unexpected error content";
      return false;
    }
    QJsonObject evobj = ev.toObject();
    QString ecode = "";
    if (evobj.contains("code")) {
      ecode = evobj.value("code").toString();
    }
    QString einfo = "";
    if (evobj.contains("info")) {
      einfo = evobj.value("info").toString();
    }

    errorMessage = QString("Received error: %2 (%1)").arg(ecode, einfo);
    return false;
  }

  if (!obj.contains("query")) {
    errorMessage = "Failed to get expected \"query\" object";
    return false;
  }

  QJsonValue qobjValue = obj.value("query");
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
  QJsonArray::const_iterator cmi;
  for (cmi = cmarr.constBegin(); cmi != cmarr.constEnd(); ++cmi) {
    WikiCategoryElement wce;

    QJsonObject cmo = cmi->toObject();
    QJsonValue tjv;
    tjv = cmo.value("pageid");
    if (tjv.type()!=QJsonValue::Double) {
      wce.pageId = "";
      result = false;
      errorMessage = "Received bad \"pageid\" element in \"query\\categorymembers\"";
    }
    else {
      wce.pageId = QString::number(tjv.toInt());
    }

    tjv = cmo.value("title");
    if (tjv.type()!=QJsonValue::String) {
      wce.title = "";
      result = false;
      errorMessage = "Received bad \"title\" element in \"query\\categorymembers\"";
    }
    else {
      wce.title = tjv.toString();
    }

    elements.append(wce);
  }

  return result;
}

// === =======================================================================

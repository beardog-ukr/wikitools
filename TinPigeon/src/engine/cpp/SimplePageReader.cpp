#include "SimplePageReader.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QVariant>
#include <QUrl>

#include <QDebug>

// === =======================================================================
// === =======================================================================

SimplePageReader::SimplePageReader(QNetworkAccessManager* nam, QObject* parent)
               :BasicHttpReader(nam,parent) {
  urlToLoad = "";
}

// === =======================================================================

SimplePageReader::~SimplePageReader() {
// nothing to do here
}

// === =======================================================================

bool SimplePageReader::setUrlToLoad(const QString& urlStr) {
  bool result = true;
  urlToLoad = QUrl::fromUserInput(urlStr);
  if (!urlToLoad.isValid()) {
    errorMessage = QString("Invalid URL: %1: %2").arg(urlStr, urlToLoad.errorString());
    result = false;
  }

  return result;
}

// === =======================================================================

QNetworkReply* SimplePageReader::makeRequest() {
  return networkAccessManager->get(QNetworkRequest(urlToLoad));
}

// === =======================================================================

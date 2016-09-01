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
               :QObject(parent) {
  urlToLoad = "";
  errorMessage = "";
  networkAccessManager = nam;
  reply = 0;
}

// === =======================================================================

SimplePageReader::~SimplePageReader() {
  if (reply) {
    reply->deleteLater();
  }
}

// === =======================================================================

QString SimplePageReader::getErrorMessage() const {
  return errorMessage;
}

// === =======================================================================

bool SimplePageReader::start(const QString& url) {
  bool result = true;
  urlToLoad = url;

  if (reply!=0) {
    errorMessage = "network reply already exists";
    result = false;
    return result;
  }

  const QUrl qUrl = QUrl::fromUserInput(urlToLoad);
  if (!qUrl.isValid()) {
    errorMessage = QString("Invalid URL: %1: %2").arg(urlToLoad, qUrl.errorString());
    result = false;
  }
  //
  qDebug() << "SimplePageReader::start >> " << "will load " << urlToLoad;

  reply = networkAccessManager->get(QNetworkRequest(qUrl));
  connect(reply, SIGNAL(finished()), this, SLOT(processReplyFinished()));
  connect(reply, SIGNAL(readyRead()), this, SLOT(processReplyReadyRead()));

  return result;
}

// === =======================================================================

void SimplePageReader::processReplyReadyRead() {
  //
  qDebug() << "SimplePageReader::processReplyReadyRead() >>" << "here";
  receivedData.append( QString::fromUtf8(reply->readAll()) );
}

// === =======================================================================

void SimplePageReader::processReplyFinished() {
  //
  qDebug() << "SimplePageReader::processReplyFinished() >>" << "here";
  qDebug() << reply->header(QNetworkRequest::ContentTypeHeader).toString();
  qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();
  qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
  qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

  if (reply->error()) {
    errorMessage = QString("Download failed:\n%1.").arg(reply->errorString());
  }

  const QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

  if (!redirectionTarget.isNull()) {
    const QUrl redirectedUrl = QUrl::fromUserInput(urlToLoad).resolved(redirectionTarget.toUrl());
    errorMessage = QString("Redirection is not supported (%1)").arg(redirectedUrl.toString());
  }

  reply->deleteLater();
  reply = 0;

  if (errorMessage.isEmpty()) {
    emit finished();
  }
}

// === =======================================================================

QStringList SimplePageReader::getReceivedData() const {
  return receivedData;
}

// === =======================================================================

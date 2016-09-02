#include "BasicHttpReader.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QVariant>
#include <QUrl>

#include <QDebug>

// === =======================================================================
// === =======================================================================

BasicHttpReader::BasicHttpReader(QNetworkAccessManager* nam, QObject* parent)
               :QObject(parent) {
  errorMessage = "";
  networkAccessManager = nam;
  reply = 0;
}

// === =======================================================================

BasicHttpReader::~BasicHttpReader() {
  if (reply) {
    reply->deleteLater();
  }
}

// === =======================================================================

QString BasicHttpReader::getErrorMessage() const {
  return errorMessage;
}

// === =======================================================================

bool BasicHttpReader::start() {
  bool result = true;

  if (reply!=0) {
    errorMessage = "network reply already exists";
    result = false;
    return result;
  }

  reply = makeRequest();
  if (reply) {
    connect(reply, SIGNAL(finished()), this, SLOT(processReplyFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(processReplyReadyRead()));
  }
  else {
    if (errorMessage.isEmpty()) {
      errorMessage = "Failed to prepare request (unknown error).";
    }
    result = false;
  }

  return result;
}

// === =======================================================================

void BasicHttpReader::processReplyReadyRead() {
  //
  qDebug() << "BasicHttpReader::processReplyReadyRead() >>" << "here";
  if (reply) {
    receivedData.append( reply->readAll() ) ;
  }
}

// === =======================================================================

void BasicHttpReader::processReplyFinished() {
  if (reply==0) {
    return;
  }
  //
  //qDebug() << "BasicHttpReader::processReplyFinished() >>" << "here";
  //qDebug() << reply->header(QNetworkRequest::ContentTypeHeader).toString();
  //qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();
  //qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
  qDebug() << "BasicHttpReader::processReplyFinished() >> " << "http reply "
           << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

  if (reply->error()) {
    errorMessage = QString("Download failed: %1 (%2)")
                      .arg(reply->errorString(), reply->url().toString());
  }

  const QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

  if (!redirectionTarget.isNull()) {
    const QUrl rtu = redirectionTarget.toUrl();
    const QUrl redirectedUrl = reply->url().resolved(rtu);
    errorMessage = QString("Redirection is not supported (%1 to %2)")
                      .arg(rtu.toString(), redirectedUrl.toString());
  }

  if (errorMessage.isEmpty()) {
    bool tboo = processReceivedData();
    if (!tboo) {
      qDebug() << "reply was " << reply->url().toString() ;
    }
  }

  reply->deleteLater();
  reply = 0;

  emit finished();
}

// === =======================================================================

QByteArray BasicHttpReader::getReceivedData() const {
  return receivedData;
}

// === =======================================================================

bool BasicHttpReader::processReceivedData() {
  //nothing to do here; should be reimplemented in other classes
  return true;
}

// === =======================================================================

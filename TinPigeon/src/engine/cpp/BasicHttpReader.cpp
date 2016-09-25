#include "BasicHttpReader.h"

#include "FiveCatsLogger.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QVariant>
#include <QUrl>

// === =======================================================================
// === =======================================================================

BasicHttpReader::BasicHttpReader(QNetworkAccessManager* nam, QObject* parent)
               :QObject(parent) {
  errorMessage = "";
  networkAccessManager = nam;
  reply = 0;

  c5 = 0;
}

// === =======================================================================

BasicHttpReader::~BasicHttpReader() {
  if (reply) {
    reply->deleteLater();
  }

  //NOTE: c5 will be deleted by main application
}

// === =======================================================================

QString BasicHttpReader::getErrorMessage() const {
  return errorMessage;
}

// === =======================================================================

void BasicHttpReader::setLogger(FiveCatsLogger* c5p) {
  c5 = c5p ;
}

// === =======================================================================

QVariant BasicHttpReader::getCookieHeader() const {
  return cookieHeader;
}

// === =======================================================================

bool BasicHttpReader::start() {
  bool result = true;

  if (reply!=0) {
    errorMessage = "network reply already exists";
    result = false;
    return result;
  }

  receivedData.clear();

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
  c5t(c5, __c5_MN__, "here");
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
  c5t(c5, __c5_MN__, "http reply " + reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString() );

  cookieHeader = reply->header(QNetworkRequest::SetCookieHeader);

//  QList<QNetworkReply::RawHeaderPair> rhplist = reply->rawHeaderPairs();
//  c5d(c5, QString("Got %1 raw headers").arg(rhplist.count()));
//  QList<QNetworkReply::RawHeaderPair>::const_iterator rhpi = rhplist.constBegin() ;
//  for (;rhpi!=rhplist.constEnd(); rhpi++) {
//    QString tmps = QString("%1 : %2").arg(QString(rhpi->first)).arg(QString(rhpi->second)) ;
//    c5d(c5, tmps);
//  }
//  c5d(c5, "== == == == == == == == == == == == ==");


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
      c5d(c5, __c5_MN__, "reply was " + reply->url().toString() );
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

bool BasicHttpReader::hasError() const {
  return !errorMessage.isEmpty();
}

// === =======================================================================

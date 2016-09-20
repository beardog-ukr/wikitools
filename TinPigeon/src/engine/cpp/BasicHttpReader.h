#ifndef __BasicHttpReader__
#define __BasicHttpReader__

#include <QObject>
#include <QString>

class FiveCatsLogger;

class QNetworkAccessManager;
class QNetworkReply;
class QByteArray;

class BasicHttpReader : public QObject
{
  Q_OBJECT

public:
  BasicHttpReader(QNetworkAccessManager* nam, QObject* parent =0);
  virtual ~BasicHttpReader();

  bool hasError() const;
  QString getErrorMessage() const;
  QByteArray getReceivedData() const;

  virtual bool start();

  void setLogger(FiveCatsLogger* c5p);

protected slots:
  virtual void processReplyReadyRead();
  virtual void processReplyFinished();

signals:
  void finished();

protected :
  QString errorMessage;

  QNetworkAccessManager* networkAccessManager;
  QNetworkReply* reply;

  QByteArray receivedData;

  virtual QNetworkReply* makeRequest() =0;
  virtual bool processReceivedData() =0;

  FiveCatsLogger* c5;
};

#endif

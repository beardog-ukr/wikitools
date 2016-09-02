#ifndef __BasicHttpReader__
#define __BasicHttpReader__

#include <QObject>
#include <QString>
//#include <QStringList>

class QNetworkAccessManager;
class QNetworkReply;
class QByteArray;

class BasicHttpReader : public QObject
{
  Q_OBJECT

public:
  BasicHttpReader(QNetworkAccessManager* nam, QObject* parent =0);
  virtual ~BasicHttpReader();

  QString getErrorMessage() const;
  QByteArray getReceivedData() const;

  virtual bool start();

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
  virtual bool processReceivedData();

};

#endif

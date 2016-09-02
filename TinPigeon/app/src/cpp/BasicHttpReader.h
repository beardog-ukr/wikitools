#ifndef __BasicHttpReader__
#define __BasicHttpReader__

#include <QObject>
#include <QString>
#include <QStringList>

class QNetworkAccessManager;
class QNetworkReply;
class QStringList;

class BasicHttpReader : public QObject
{
  Q_OBJECT

public:
  BasicHttpReader(QNetworkAccessManager* nam, QObject* parent =0);
  virtual ~BasicHttpReader();

  QString getErrorMessage() const;
  QStringList getReceivedData() const;

  virtual bool start();

protected slots:
  void processReplyReadyRead();
  void processReplyFinished();

signals:
  void finished();

protected :
  QString errorMessage;

  QNetworkAccessManager* networkAccessManager;
  QNetworkReply* reply;

  QStringList receivedData;

  virtual QNetworkReply* makeRequest() =0;

};

#endif

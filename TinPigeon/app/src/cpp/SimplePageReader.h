#ifndef __SimplePageReader__
#define __SimplePageReader__

#include <QObject>
#include <QString>
#include <QStringList>

class QNetworkAccessManager;
class QNetworkReply;
class QStringList;

class SimplePageReader : public QObject
{
  Q_OBJECT

public:
  SimplePageReader(QNetworkAccessManager* nam, QObject* parent =0);
  virtual ~SimplePageReader();

  QString getErrorMessage() const;
  QStringList getReceivedData() const;

  bool start(const QString& url);

protected slots:
  void processReplyReadyRead();
  void processReplyFinished();

signals:
  void finished();

protected :
  QString urlToLoad;
  QString errorMessage;

  QNetworkAccessManager* networkAccessManager;
  QNetworkReply* reply;

  QStringList receivedData;

};

#endif

#ifndef __SimplePageReader__
#define __SimplePageReader__

#include "BasicHttpReader.h"

#include <QUrl>

class SimplePageReader : public BasicHttpReader
{
  Q_OBJECT

public:
  SimplePageReader(QNetworkAccessManager* nam, QObject* parent =0);
  virtual ~SimplePageReader();

  bool setUrlToLoad(const QString& urlStr);

protected :
  QNetworkReply* makeRequest();

  QUrl urlToLoad;
};

#endif

#ifndef __WikiExtLinksReader__
#define __WikiExtLinksReader__

#include "BasicHttpReader.h"

#include <QStringList>

// === =======================================================================

// Here is the request example
// https://en.wikipedia.org/w/api.php?
//       action=query&format=json&prop=extlinks&pageids=31972164

class WikiExtLinksReader : public BasicHttpReader
{
  Q_OBJECT

public:
  WikiExtLinksReader(QNetworkAccessManager* nam, QObject* parent =0);
  virtual ~WikiExtLinksReader();

  bool setWiki(const QString& langCode);
  bool setPageId(const int& category);

  QStringList getExternalLinks() const;

protected :
  virtual QNetworkReply* makeRequest();
  virtual bool processReceivedData();

  QString langCode;
  int pageId;

  QStringList externalLinks;
};

// === =======================================================================

#endif

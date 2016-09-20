#ifndef __ArticleWikitextReader__
#define __ArticleWikitextReader__

#include "BasicHttpReader.h"

#include <QStringList>

// === =======================================================================

class ArticleWikitextReader : public BasicHttpReader
{
  Q_OBJECT

public:
  ArticleWikitextReader(QNetworkAccessManager* nam, QObject* parent =0);
  virtual ~ArticleWikitextReader();

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

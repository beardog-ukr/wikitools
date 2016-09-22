#ifndef __ArticleWikitextReader__
#define __ArticleWikitextReader__

#include "BasicHttpReader.h"

#include <QStringList>

// === =======================================================================

class ArticleWikitextReader : public BasicHttpReader
{
  Q_OBJECT
  friend class TestEngine;
public:
  ArticleWikitextReader(QNetworkAccessManager* nam, QObject* parent =0);
  virtual ~ArticleWikitextReader();

  void resetWikitext();
  QString getWikitext() const;

  void setWiki(const QString& langCode);
  void setPageTitle(const QString& pageTitle);
  QString getPageTitle() const;

  QStringList getExternalLinks() const;

protected :
  ArticleWikitextReader();
  void setupAll() ;

  virtual QNetworkReply* makeRequest();
  virtual bool processReceivedData();

  bool readWikitextFromJson(QJsonObject* rootObj);

  QString langCode;
  QString pageTitle;

  QString wikitext;
};

// === =======================================================================

#endif

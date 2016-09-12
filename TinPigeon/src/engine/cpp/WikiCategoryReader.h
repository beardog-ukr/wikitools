#ifndef __WikiCategoryReader__
#define __WikiCategoryReader__

#include "BasicHttpReader.h"

#include <QList>

// === =======================================================================

struct WikiCategoryElement {
//public:
  QString title;
  QString pageId;
};

// === =======================================================================

class WikiCategoryReader : public BasicHttpReader
{
  Q_OBJECT

public:
  WikiCategoryReader(QNetworkAccessManager* nam, QObject* parent =0);
  virtual ~WikiCategoryReader();

  bool setWiki(const QString& langCode);
  bool setCategory(const QString& category);

  QList<WikiCategoryElement> getCategoryElements() const;

protected :
  virtual QNetworkReply* makeRequest();
  virtual bool processReceivedData();

  QString langCode;
  QString category;

  QList<WikiCategoryElement> elements;
};

// === =======================================================================

#endif

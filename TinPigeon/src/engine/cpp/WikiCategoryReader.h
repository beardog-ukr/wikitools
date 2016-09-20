#ifndef __WikiCategoryReader__
#define __WikiCategoryReader__

#include "BasicHttpReader.h"
// === =======================================================================
#include <QStringList>
class QJsonDocument;

// === =======================================================================

class WikiCategoryReader : public BasicHttpReader
{
  Q_OBJECT

  friend class TestEngine;

public:
  WikiCategoryReader(QNetworkAccessManager* nam, QObject* parent =0);
  virtual ~WikiCategoryReader();

  bool setWiki(const QString& langCode);
  bool setCategory(const QString& category);
  bool setContinueId(const QString& cid);
  bool setRecordsLimit(const int limit);

  QString getNextContinueId() const;
  QStringList getCategoryTitles() const;

  void resetCategoryTitles();

protected :
  WikiCategoryReader();

  void setupAll();

  virtual QNetworkReply* makeRequest();
  virtual bool processReceivedData();

  QString langCode;
  QString category;
  QString currentContinueId;
  int recordsLimit;

  QStringList titles;
  QString nextContinueId;

  bool readElementsFromJsonDoc(QJsonObject* rootObj, QStringList& elements);
  bool readErrorFromJsonDoc(QJsonObject* rootObj);
  bool readContinueId(QJsonObject* rootObj);
};

// === =======================================================================

#endif

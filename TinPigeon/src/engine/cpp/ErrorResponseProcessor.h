#ifndef __ErrorResponseProcessor__
#define __ErrorResponseProcessor__

class FiveCatsLogger;

class QJsonObject;

#include <QString>

// === =======================================================================

class ErrorResponseProcessor
{
  friend class TestEngine;
public:
  ErrorResponseProcessor();
  ErrorResponseProcessor(FiveCatsLogger* c5);
  virtual ~ErrorResponseProcessor();

  void loadJson(QJsonObject* jobj);

  bool hasError();

  QString errorCode() const;
  QString errorInfo() const;

protected :
  QString code;
  QString info;
  bool foundError;

  FiveCatsLogger* c5;
};

// === =======================================================================

#endif

#include "ErrorResponseProcessor.h"
#include "FiveCatsLogger.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>

#include <QVariant>

// === =======================================================================
// === =======================================================================

ErrorResponseProcessor::ErrorResponseProcessor() {
  info = "";
  code = "";
  foundError = false;
  c5 =0;
}

// === =======================================================================

ErrorResponseProcessor::ErrorResponseProcessor(FiveCatsLogger* c5p) {
  info = "";
  code = "";
  foundError = false;
  c5 =c5p;
}

// === =======================================================================

ErrorResponseProcessor::~ErrorResponseProcessor()  {
  //
}

// === =======================================================================

void ErrorResponseProcessor::loadJson(QJsonObject* obj) {
  foundError = false ;

  if (!obj->contains("error")) {
    return ;
  }

  QJsonValue ev = obj->value("error");
  if (!ev.isObject()) {
    return ;
  }

  foundError = true;

  QJsonObject evobj = ev.toObject();
  QString ecode = "";
  if (evobj.contains("code")) {
    code = evobj.value("code").toString();
  }
  QString einfo = "";
  if (evobj.contains("info")) {
    info = evobj.value("info").toString();
  }

  if (ecode.isEmpty() && einfo.isEmpty()) {
    c5d(c5, __c5_MN__, "Bad error message format");
  }
}

// === =======================================================================

bool ErrorResponseProcessor::hasError() {
  return foundError;
}

// === =======================================================================

QString ErrorResponseProcessor::errorCode() const {
  return code;
}

// === =======================================================================

QString ErrorResponseProcessor::errorInfo() const {
  return info;
}

// === =======================================================================
// === =======================================================================

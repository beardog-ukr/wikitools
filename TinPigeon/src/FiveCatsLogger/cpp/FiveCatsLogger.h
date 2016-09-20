#ifndef __FiveCatsLogger__
#define __FiveCatsLogger__

#include <string>
#include <QString>
#include <QCommandLineParser>

class QFile;

// === ========================================================================

class FiveCatsLogger {
  friend class TestFiveCatsLogger;
public:
  enum LogLevel {
    Critical = 0,
    Warning ,
    Info,
    Debug,
    Trace,
    Flood
  };

  FiveCatsLogger();
  FiveCatsLogger(LogLevel logLevel);
  virtual ~FiveCatsLogger();

  bool setFileName(const QString& fileName, bool append = false) ;

  void setUseStdOut(bool use);
  bool usesStdOut() const;

  bool setKeepFileExtension(const bool keep);

  QString getErrorMessage() const ;

  void setLogLevel(LogLevel logLevel) ;

  static void initCommandLineParser(QCommandLineParser& parser);
  void loadCommandLineParser(const QCommandLineParser& parser);

  void c(const QString& message);
  void c(const QString& metaInfo, const QString& message) ;
  void w(const QString& message);
  void w(const QString& metaInfo, const QString& message) ;
  void i(const QString& message);
  void i(const QString& metaInfo, const QString& message) ;
  void d(const QString& message);
  void d(const QString& metaInfo, const QString& message) ;
  void t(const QString& message);
  void t(const QString& metaInfo, const QString& message) ;
  void f(const QString& message);
  void f(const QString& metaInfo, const QString& message) ;

protected:
  void selfSetup();

  LogLevel logLevel;

  void write(LogLevel messageLevel, const QString str);

  void writeFile(const QString& metaInfo, const QString& message);

  QFile* file;
  int flushCounter;
  const int flushFrequency = 20;
  QString prepareFileName(const QString& fn) const;
  QString prepareNewFileName(const QString& fn) const;

  QFile* echo;

  QString errorMessage;

  bool useStdout;
  QString composeLine(const QString& metaInfo, const QString& message) const;

  bool keepFileExtension;
};

// === ========================================================================

void c5c(FiveCatsLogger* logger, const QString message) ;
void c5c(FiveCatsLogger* logger, const QString metaInfo, const QString message) ;
void c5w(FiveCatsLogger* logger, const QString message) ;
void c5w(FiveCatsLogger* logger, const QString metaInfo, const QString message) ;
void c5i(FiveCatsLogger* logger, const QString message) ;
void c5i(FiveCatsLogger* logger, const QString metaInfo, const QString message) ;
void c5d(FiveCatsLogger* logger, const QString message) ;
void c5d(FiveCatsLogger* logger, const QString metaInfo, const QString message) ;
void c5t(FiveCatsLogger* logger, const QString message) ;
void c5t(FiveCatsLogger* logger, const QString metaInfo, const QString message) ;
void c5f(FiveCatsLogger* logger, const QString message) ;
void c5f(FiveCatsLogger* logger, const QString metaInfo, const QString message) ;

// === ========================================================================
// this came from http://stackoverflow.com/questions/1666802/is-there-a-class-macro-in-c
inline QString c5_methodName(const std::string& prettyFunction) {
    size_t colons = prettyFunction.find("::");
    size_t begin = prettyFunction.substr(0,colons).rfind(" ") + 1;
    size_t end = prettyFunction.rfind("(") - begin;

    std::string ss = prettyFunction.substr(begin,end) + "()";
    return QString::fromStdString(ss);
}

#define __c5_MN__ c5_methodName(__PRETTY_FUNCTION__)

// === ========================================================================

#endif

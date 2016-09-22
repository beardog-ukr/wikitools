#include "ActionWikitextREChecker.h"

#include "FiveCatsLogger.h"
#include "ArticleWikitextReader.h"

// === =======================================================================

#include <QByteArray>
#include <QCommandLineParser>
#include <QFile>
#include <QMap>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QTextCodec>

// === =======================================================================
// === =======================================================================

ActionWikitextREChecker::ActionWikitextREChecker(QObject *parent)
                        :ActionBasic(parent){
  wikiLanguage = "en";
  titlesFileName = "";
  reportFileName = "";

  pageReader = 0 ;
  networkAccessManager = 0; //
}

// === =======================================================================

ActionWikitextREChecker::~ActionWikitextREChecker() {
  delete networkAccessManager;
  delete pageReader;
}

// === =======================================================================

void ActionWikitextREChecker::initCommandLineParser(QCommandLineParser& parser) {
  // command line options
  QCommandLineOption titlesOption("recp_titles",
      "Text file with list of titles to process", "recp_titles");
  parser.addOption(titlesOption);

  QCommandLineOption rfnOption("recp_report",
      "File to save the report of performe actions", "recp_report");
  parser.addOption(rfnOption);

  QCommandLineOption reOption("recp_re", "RexExp to test", "recp_re");
  parser.addOption(reOption);

  QCommandLineOption wlOption("recp_lang", "Wiki language", "recp_lang");
  parser.addOption(wlOption);
}

// === =======================================================================

bool ActionWikitextREChecker::loadCommandLineParser(const QCommandLineParser& parser) {
  QString tmps;

  tmps = parser.value("recp_titles");
  if (!tmps.isEmpty()) {
    titlesFileName = tmps ;
  }

  tmps = parser.value("recp_report");
  if (!tmps.isEmpty()) {
    reportFileName = tmps ;
  }

  tmps = parser.value("recp_re");
  if (!tmps.isEmpty()) {
    regExp.setPattern(tmps) ;
  }

  tmps = parser.value("recp_lang");
  if (!tmps.isEmpty()) {
    wikiLanguage = tmps ;
  }

  //Some initial tests
  if (titlesFileName.isEmpty()) {
    errorMessage = "Page titles file must be specified";
    return false;
  }

  if ( !regExp.isValid() ) {
    errorMessage = "Invalid pattern for regular expression";
    return false;
  }

  return true;
}

// === =======================================================================

bool ActionWikitextREChecker::loadPageTitles() {
  if (titlesFileName.isEmpty()) {
    errorMessage = "No titles file name received";
    return false;
  }

  QFile fl(titlesFileName);
  if ( !fl.open(QFile::ReadOnly)) {
    errorMessage = QString("Problem opening file (%1)").arg(titlesFileName);
    return false;
  }

  QTextStream stream(&fl);
  stream.setCodec( "UTF-8" );
  QString line;
  while (stream.readLineInto(&line)) {
    titlesToProcess.append(line);
  }

  fl.close();

  //finally
  return true;
}

// === =======================================================================

bool ActionWikitextREChecker::startAction() {
  if (!loadPageTitles()) {
    return false;
  }

  networkAccessManager = new QNetworkAccessManager(this);

  c5d(c5, __c5_MN__, "Going to start");

  pageReader = new ArticleWikitextReader(networkAccessManager, this);
  pageReader->setLogger(c5);
  pageReader->setWiki(wikiLanguage);
  connect(pageReader, SIGNAL(finished()), this, SLOT(processResult()) ) ;

  const bool started = startNextTitle();
  return started;
}

// === =======================================================================

void ActionWikitextREChecker::processResult() {
  const QString em = pageReader->getErrorMessage();
  if (!em.isEmpty()) {
    errorMessage = em;
    emit actionFailure();
    return;
  }

  QString wtStr = pageReader->getWikitext();

  const QString ptstr = pageReader->getPageTitle();
  checkForRE(ptstr, wtStr);
  processedTitles.append(ptstr);
  c5i(c5, "Processed " + ptstr);

  if (titlesToProcess.isEmpty()) {
    processFinalResult();
    return;
  }
  else {
    if (!startNextTitle()) {
      emit actionFailure();
    }
    return;
  }
}

// === =======================================================================

bool ActionWikitextREChecker::checkForRE(const QString& title,const QString& data) {
  QStringList sl = data.split("\n");

  QStringList::const_iterator ri = sl.constBegin();
  for (; ri!=sl.constEnd(); ri++) {
    QRegularExpressionMatch m = regExp.match(*ri) ;
    if (m.hasMatch()) {
      QPair<QString, QString> p(title, *ri);
      matches.append(p);
      c5t(c5, __c5_MN__, "Added Match " + (*ri));
    }
  }

  return true;
}

// === =======================================================================

bool ActionWikitextREChecker::startNextTitle() {
  QString ss= titlesToProcess.takeFirst();
  pageReader->setPageTitle(ss) ;

  pageReader->resetWikitext();
  bool started = pageReader->start();
  if (!started) {
    errorMessage = pageReader->getErrorMessage();
    delete pageReader;
    pageReader = 0;
  }

  return started;
}

// === =======================================================================

void ActionWikitextREChecker::processFinalResult() {
  //
  QString rs = "" ;
  rs += QString("Processed %1 titles:\n").arg(processedTitles.count());

  QStringList::const_iterator pti = processedTitles.constBegin();
  for (; pti!=processedTitles.constEnd(); pti++) {
    rs += *pti ;
    rs += "\n" ;
  }

  rs += QString("Regular expression to test was \"%1\"\n").arg(regExp.pattern());
  rs += QString("Found %1 matches:\n").arg(matches.count());

  QList<QPair<QString, QString> >::const_iterator mi = matches.constBegin();
  for (; mi!=matches.constEnd(); mi++) {
    rs += QString("Article \"%1\" : \"%2\"\n").arg(mi->first).arg(mi->second) ;
  }

  c5i(c5, rs);

  //TODO: write to
  QFile fl(reportFileName);
  bool tboo = fl.open(QIODevice::WriteOnly | QIODevice::Text);
  if (!tboo) {
    errorMessage = "Failed to open " + reportFileName;
    emit actionFailure();
    return;
  }
  fl.write( rs.toUtf8() ) ;
  fl.flush();
  fl.close();

  emit actionDone();
}

// === =======================================================================
// === =======================================================================

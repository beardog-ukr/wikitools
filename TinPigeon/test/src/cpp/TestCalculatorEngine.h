#include <QtTest/QtTest>
#include <QList>

#include <string>

// === =======================================================================
class CalculatorEngine;

// === =======================================================================
// === =======================================================================

class TestCalculatorEngine: public QObject
{
  Q_OBJECT
private slots:
  void getScreenText_BasicA();
  void getScreenText_BasicB();
  void getScreenText_FloatZero();

  void prepareScreenText_Integer();
  void prepareScreenText_BasicFloat();
  void prepareScreenText_LargeFracFloatA();
  void prepareScreenText_LargeFracFloatA2();
  void prepareScreenText_LargeFracFloatB();
  void prepareScreenText_LargeFracFloatC();
  void prepareScreenText_AlmostZeroFloat();
  void prepareScreenText_TooLargeA();
  void prepareScreenText_TooLargeB();
  void prepareScreenText_TooLargeC();

  void actionsScenario_EnterNumInt();
  void actionsScenario_EnterNumFloatA();
  void actionsScenario_EnterNumFloatB();
  void actionsScenario_EnterNumFloatC();
  void actionsScenario_EnterNumLargeInt();
  void actionsScenario_EnterNumLargeFloat();

  void actionsScenario_BasicAdd();
  void actionsScenario_BasicSub();
  void actionsScenario_BasicDiv();
  void actionsScenario_BasicMultiply();

  void actionsScenario_SameMultiply();
  void actionsScenario_SameMultiplySeqAdd();

  void actionsScenario_SeqAdd();
  void actionsScenario_SeqAddSub();
  void actionsScenario_SeqAddSubNoClear() ;
  void actionsScenario_SeqAddResultSub();

  void actionsScenario_MemSetUseA();
  void actionsScenario_MemSetUseB();
  void actionsScenario_MemSetClearA();
  void actionsScenario_MemSetClearB() ;
  void actionsScenario_MemSetUseAdd();
  void actionsScenario_MemSetUseSub();
  void actionsScenario_MemSetAfterResultA();
  void actionsScenario_MemSetAfterResultB();
  void actionsScenario_MemSetAfterAction();
  void actionsScenario_MemUseBadFloatA() ;
  void actionsScenario_MemUseMemPlusA() ;
  void actionsScenario_MemUseMemPlusB() ;

};

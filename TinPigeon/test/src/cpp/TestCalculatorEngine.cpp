#include "TestCalculatorEngine.h"

#include "CalculatorEngine.h"

#include <QList>
#include <QStringList>

// === ========================================================================
// === ========================================================================

class SamplesKeeper {

protected:
  QList<std::string> buttonIds;
  QList<QString> displayStates;
  QList<bool> memoryIndicatorStates;
  int currentIdx;

public:
  SamplesKeeper();

  void add(const std::string buttinId, const QString displayState,
           const bool memoryIndicatorState)  ;
  int count() const;

  std::string buttonId() const;
  QString displayState() const;
  bool memoryIndicatorState() const;
  void goNext();
};

SamplesKeeper::SamplesKeeper() {
  currentIdx = 0;
}

void SamplesKeeper::add(const std::string buttonId, const QString displayState,
           const bool memoryIndicatorState)  {
  buttonIds.append(buttonId);
  displayStates.append(displayState);
  memoryIndicatorStates.append(memoryIndicatorState);
}

int SamplesKeeper::count() const {
  return memoryIndicatorStates.count();
}

std::string SamplesKeeper::buttonId() const {
  return buttonIds.at(currentIdx);
}

QString SamplesKeeper::displayState() const {
  return displayStates.at(currentIdx);
}

bool SamplesKeeper::memoryIndicatorState() const {
  return memoryIndicatorStates.at(currentIdx);
}

void SamplesKeeper::goNext() {
  currentIdx++;
}

void processSamples(SamplesKeeper& sk) {
  CalculatorEngine ce;
  for (int i=0; i<sk.count(); i++) {
    QString buttonId = QString::fromStdString(sk.buttonId());
    QString displayState = sk.displayState();
    bool memoryIndicatorState = sk.memoryIndicatorState();

    ce.processOperation(buttonId);
    QCOMPARE(ce.getScreenText(), displayState );
    QCOMPARE(ce.getMemoryState(), memoryIndicatorState );

    sk.goNext();
  }
}

// === ========================================================================
// === ========================================================================

void TestCalculatorEngine::actionsScenario_SameMultiply() {
  SamplesKeeper sk;
  sk.add(CalculatorEngine::digit5, "5", false);
  sk.add(CalculatorEngine::opMult, "5", false);
  sk.add(CalculatorEngine::opResult, "25", false);

  processSamples(sk);
}

// === ========================================================================

void TestCalculatorEngine::actionsScenario_SameMultiplySeqAdd() {
  SamplesKeeper sk;
  sk.add(CalculatorEngine::digit5, "5", false);
  sk.add(CalculatorEngine::opMult, "5", false);
  sk.add(CalculatorEngine::opResult, "25", false);
  sk.add(CalculatorEngine::opPlus,  "25", false);
  sk.add(CalculatorEngine::digit7,  "7", false);
  sk.add(CalculatorEngine::opResult, "32", false);

  processSamples(sk) ;
}

// === ========================================================================

void TestCalculatorEngine::getScreenText_BasicA() {
  CalculatorEngine calculatorEngine;
  QString sv = calculatorEngine.getScreenText();
  QCOMPARE(sv, QString("0") );
}

// === ========================================================================

void TestCalculatorEngine::getScreenText_BasicB() {
  CalculatorEngine calculatorEngine;
  calculatorEngine.totalDigitCounter = 5;
  calculatorEngine.currentOperandInt = 12345;
  calculatorEngine.engineStage=CalculatorEngine::ES_receiving_int_1;
  calculatorEngine.fracNum = 0;
  QString sv = calculatorEngine.getScreenText();
  QCOMPARE(sv, QString("12345") );
}

// === ========================================================================

void TestCalculatorEngine::getScreenText_FloatZero() {
  CalculatorEngine calculatorEngine;
  calculatorEngine.totalDigitCounter = 5;
  calculatorEngine.currentOperandInt = 12;
  calculatorEngine.currentOperandFrac = 300;
  calculatorEngine.engineStage=CalculatorEngine::ES_receiving_frac_1;
  calculatorEngine.fracNum = 3;
  QString sv = calculatorEngine.getScreenText();
  QCOMPARE(sv, QString("12,300") );
}

// === ========================================================================

void TestCalculatorEngine::prepareScreenText_Integer() {
  CalculatorEngine calculatorEngine;
  const float value = 23.0;
  QString sv = calculatorEngine.prepareScreenText(value);
  QCOMPARE(sv, QString("23") );
}

// === ========================================================================

void TestCalculatorEngine::prepareScreenText_BasicFloat() {
  CalculatorEngine calculatorEngine;
  const float value = 23.5;
  QString sv = calculatorEngine.prepareScreenText(value);
  QCOMPARE(sv, QString("23,5") );
}

// === ========================================================================

void TestCalculatorEngine::prepareScreenText_LargeFracFloatA() {
  CalculatorEngine calculatorEngine;
  const float value =123.4567337123456;
  QString sv = calculatorEngine.prepareScreenText(value);
  QCOMPARE(sv, QString("123,4567337") );
}

// === ========================================================================
// This test is the same as LargeFracFloatA, but the number is different.
// This test fails if value uses "float" data type.
void TestCalculatorEngine::prepareScreenText_LargeFracFloatA2() {
  CalculatorEngine calculatorEngine;
  const double value =123.456789112345678;
  QString sv = calculatorEngine.prepareScreenText(value);
  QCOMPARE(sv, QString("123,4567891") );
}

// === ========================================================================

void TestCalculatorEngine::prepareScreenText_LargeFracFloatB() {
  CalculatorEngine calculatorEngine;
  const float value = 123.000000034;
  QString sv = calculatorEngine.prepareScreenText(value);
  QCOMPARE(sv, QString("123") );
}

// === ========================================================================
// This test is the same as LargeFracFloatA, but the number is different.
// Note zeroes after truncated string
void TestCalculatorEngine::prepareScreenText_LargeFracFloatC() {
  CalculatorEngine calculatorEngine;

  double value =123.45678009112345678;
  QString sv = calculatorEngine.prepareScreenText(value);
  QCOMPARE(sv, QString("123,4567801") );

  value =123.456700009112345678;
  sv = calculatorEngine.prepareScreenText(value);
  QCOMPARE(sv, QString("123,4567") );

  value =1.001;
  sv = calculatorEngine.prepareScreenText(value);
  QCOMPARE(sv, QString("1,001") );
}

// === ========================================================================
//
void TestCalculatorEngine::prepareScreenText_TooLargeA() {
  CalculatorEngine calculatorEngine;
  const double value =12345678900;
  QString sv = calculatorEngine.prepareScreenText(value);
  QCOMPARE(sv, QString("Err") );
}

// === ========================================================================
//
void TestCalculatorEngine::prepareScreenText_TooLargeB() {
  CalculatorEngine calculatorEngine;
  const double value =1234567890.1234;
  QString sv = calculatorEngine.prepareScreenText(value);
  QCOMPARE(sv, QString("Err") );
}

// === ========================================================================
//
void TestCalculatorEngine::prepareScreenText_TooLargeC() {
  CalculatorEngine calculatorEngine;
  const double value =123456789.1234;
  QString sv = calculatorEngine.prepareScreenText(value);
  QCOMPARE(sv, QString("123456789,1") );
}

// === ========================================================================

void TestCalculatorEngine::prepareScreenText_AlmostZeroFloat() {
  CalculatorEngine calculatorEngine;
  const double value = 0.000000000023;
  QString sv = calculatorEngine.prepareScreenText(value);
  QCOMPARE(sv, QString("0") );
}

// === ========================================================================

void TestCalculatorEngine::actionsScenario_EnterNumInt() {
  CalculatorEngine calculatorEngine;

  calculatorEngine.processOperation("d9");
  QCOMPARE(calculatorEngine.getScreenText(), QString("9") );
  calculatorEngine.processOperation("d3");
  QCOMPARE(calculatorEngine.getScreenText(), QString("93") );
}

// === ========================================================================

void TestCalculatorEngine::actionsScenario_EnterNumFloatA() {
  CalculatorEngine calculatorEngine;

  calculatorEngine.processOperation("d9");
  QCOMPARE(calculatorEngine.getScreenText(), QString("9") );
  calculatorEngine.processOperation("d3");
  QCOMPARE(calculatorEngine.getScreenText(), QString("93") );
  calculatorEngine.processOperation( QString::fromStdString(calculatorEngine.digitDot) );
  QCOMPARE(calculatorEngine.getScreenText(), QString("93") );
  calculatorEngine.processOperation("d5");
  QCOMPARE(calculatorEngine.getScreenText(), QString("93,5") );
}

// === ========================================================================

void TestCalculatorEngine::actionsScenario_EnterNumFloatB() {
  CalculatorEngine calculatorEngine;

  calculatorEngine.processOperation("d9");
  QCOMPARE(calculatorEngine.getScreenText(), QString("9") );
  calculatorEngine.processOperation( QString::fromStdString(calculatorEngine.digitDot) );
  QCOMPARE(calculatorEngine.getScreenText(), QString("9") );
  calculatorEngine.processOperation("d5");
  QCOMPARE(calculatorEngine.getScreenText(), QString("9,5") );
  calculatorEngine.processOperation("d0");
  QCOMPARE(calculatorEngine.getScreenText(), QString("9,50") );
  QCOMPARE(calculatorEngine.currentOperandInt, 9 );
  QCOMPARE(calculatorEngine.currentOperandFrac, 50 );
  calculatorEngine.processOperation("d5");
  QCOMPARE(calculatorEngine.getScreenText(), QString("9,505") );
  QCOMPARE(calculatorEngine.currentOperandInt, 9 );
  QCOMPARE(calculatorEngine.currentOperandFrac, 505 );
}

// === ========================================================================
// This test is for issue #2
void TestCalculatorEngine::actionsScenario_EnterNumFloatC() {
  CalculatorEngine calculatorEngine;

  calculatorEngine.processOperation("d9");
  QCOMPARE(calculatorEngine.getScreenText(), QString("9") );
  calculatorEngine.processOperation( QString::fromStdString(calculatorEngine.digitDot) );
  QCOMPARE(calculatorEngine.getScreenText(), QString("9") );
  calculatorEngine.processOperation("d0");
  QCOMPARE(calculatorEngine.getScreenText(), QString("9,0") );
  calculatorEngine.processOperation("d0");
  QCOMPARE(calculatorEngine.getScreenText(), QString("9,00") );
  calculatorEngine.processOperation("d5");
  QCOMPARE(calculatorEngine.getScreenText(), QString("9,005") );

}

// === ========================================================================

void TestCalculatorEngine::actionsScenario_EnterNumLargeInt() {
  CalculatorEngine calculatorEngine;

  calculatorEngine.processOperation("d1");
  QCOMPARE(calculatorEngine.getScreenText(), QString("1") );
  calculatorEngine.processOperation("d2");
  QCOMPARE(calculatorEngine.getScreenText(), QString("12") );
  calculatorEngine.processOperation("d3");
  QCOMPARE(calculatorEngine.getScreenText(), QString("123") );
  calculatorEngine.processOperation("d4");
  QCOMPARE(calculatorEngine.getScreenText(), QString("1234") );
  calculatorEngine.processOperation("d5");
  QCOMPARE(calculatorEngine.getScreenText(), QString("12345") );
  calculatorEngine.processOperation("d6");
  QCOMPARE(calculatorEngine.getScreenText(), QString("123456") );
  calculatorEngine.processOperation("d7");
  QCOMPARE(calculatorEngine.getScreenText(), QString("1234567") );
  calculatorEngine.processOperation("d8");
  QCOMPARE(calculatorEngine.getScreenText(), QString("12345678") );
  calculatorEngine.processOperation("d9");
  QCOMPARE(calculatorEngine.getScreenText(), QString("123456789") );
  calculatorEngine.processOperation("d0");
  QCOMPARE(calculatorEngine.getScreenText(), QString("1234567890") );
  //that's all, max digits
  calculatorEngine.processOperation("d1");
  QCOMPARE(calculatorEngine.getScreenText(), QString("1234567890") );
}

// === ========================================================================

void TestCalculatorEngine::actionsScenario_EnterNumLargeFloat(){
  CalculatorEngine calculatorEngine;

  calculatorEngine.processOperation("d1");
  QCOMPARE(calculatorEngine.getScreenText(), QString("1") );
  calculatorEngine.processOperation("d2");
  QCOMPARE(calculatorEngine.getScreenText(), QString("12") );
  calculatorEngine.processOperation("d3");
  QCOMPARE(calculatorEngine.getScreenText(), QString("123") );
  calculatorEngine.processOperation("d4");
  QCOMPARE(calculatorEngine.getScreenText(), QString("1234") );
  calculatorEngine.processOperation("d5");
  QCOMPARE(calculatorEngine.getScreenText(), QString("12345") );
  calculatorEngine.processOperation("d6");
  QCOMPARE(calculatorEngine.getScreenText(), QString("123456") );
  calculatorEngine.processOperation("d7");
  QCOMPARE(calculatorEngine.getScreenText(), QString("1234567") );

  calculatorEngine.processOperation( QString::fromStdString(calculatorEngine.digitDot) );
  QCOMPARE(calculatorEngine.getScreenText(), QString("1234567") );

  calculatorEngine.processOperation("d8");
  QCOMPARE(calculatorEngine.getScreenText(), QString("1234567,8") );
  calculatorEngine.processOperation("d9");
  QCOMPARE(calculatorEngine.getScreenText(), QString("1234567,89") );
  calculatorEngine.processOperation("d0");
  QCOMPARE(calculatorEngine.getScreenText(), QString("1234567,890") );
  //that's all, max digits
  calculatorEngine.processOperation("d1");
  QCOMPARE(calculatorEngine.getScreenText(), QString("1234567,890") );
}

// === ========================================================================

void TestCalculatorEngine::actionsScenario_BasicAdd() {
  CalculatorEngine ce;

  ce.processOperation("d1");
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation( QString::fromStdString(ce.digitDot) );
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation("d2");
  QCOMPARE(ce.getScreenText(), QString("1,2") );

  ce.processOperation( QString::fromStdString(ce.opPlus) );
  QCOMPARE(ce.getScreenText(), QString("1,2") );

  ce.processOperation("d3");
  QCOMPARE(ce.getScreenText(), QString("3") );
  ce.processOperation("d4");
  QCOMPARE(ce.getScreenText(), QString("34") );

  ce.processOperation( QString::fromStdString(ce.opResult) );
  QCOMPARE(ce.getScreenText(), QString("35,2") );
}

// === ========================================================================

void TestCalculatorEngine::actionsScenario_BasicSub() {
  CalculatorEngine ce;

  ce.processOperation("d9");
  QCOMPARE(ce.getScreenText(), QString("9") );
  ce.processOperation( QString::fromStdString(ce.digitDot) );
  QCOMPARE(ce.getScreenText(), QString("9") );
  ce.processOperation("d2");
  QCOMPARE(ce.getScreenText(), QString("9,2") );

  ce.processOperation( QString::fromStdString(ce.opMinus) );
  QCOMPARE(ce.getScreenText(), QString("9,2") );

  ce.processOperation("d3");
  QCOMPARE(ce.getScreenText(), QString("3") );

  ce.processOperation( QString::fromStdString(ce.opResult) );
  QCOMPARE(ce.getScreenText(), QString("6,2") );
}

// === ========================================================================

void TestCalculatorEngine::actionsScenario_BasicDiv(){
  CalculatorEngine ce;

  ce.processOperation("d1");
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation( QString::fromStdString(ce.digitDot) );
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation("d2");
  QCOMPARE(ce.getScreenText(), QString("1,2") );

  ce.processOperation( QString::fromStdString(ce.opDivide) );
  QCOMPARE(ce.getScreenText(), QString("1,2") );

  ce.processOperation("d3");
  QCOMPARE(ce.getScreenText(), QString("3") );

  ce.processOperation( QString::fromStdString(ce.opResult) );
  QCOMPARE(ce.getScreenText(), QString("0,4") );
}

// === ========================================================================

void TestCalculatorEngine::actionsScenario_BasicMultiply() {
  CalculatorEngine ce;

  ce.processOperation("d1");
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation( QString::fromStdString(ce.digitDot) );
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation("d2");
  QCOMPARE(ce.getScreenText(), QString("1,2") );

  ce.processOperation( QString::fromStdString(ce.opMult) );
  QCOMPARE(ce.getScreenText(), QString("1,2") );

  ce.processOperation("d3");
  QCOMPARE(ce.getScreenText(), QString("3") );

  ce.processOperation( QString::fromStdString(ce.opResult) );
  QCOMPARE(ce.getScreenText(), QString("3,6") );
}

// === ========================================================================

void TestCalculatorEngine::actionsScenario_SeqAdd() {
  CalculatorEngine ce;

  ce.processOperation("d1");
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation( QString::fromStdString(ce.digitDot) );
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation("d2");
  QCOMPARE(ce.getScreenText(), QString("1,2") );

  ce.processOperation( QString::fromStdString(ce.opPlus) );
  QCOMPARE(ce.getScreenText(), QString("1,2") );

  ce.processOperation("d3");
  QCOMPARE(ce.getScreenText(), QString("3") );
  ce.processOperation("d4");
  QCOMPARE(ce.getScreenText(), QString("34") );

  ce.processOperation( QString::fromStdString(ce.opPlus) );
  QCOMPARE(ce.getScreenText(), QString("35,2") );

  ce.processOperation("d5");
  QCOMPARE(ce.getScreenText(), QString("5") );


  ce.processOperation( QString::fromStdString(ce.opResult) );
  QCOMPARE(ce.getScreenText(), QString("40,2") );
}

// === ========================================================================

void TestCalculatorEngine::actionsScenario_SeqAddSub() {
  CalculatorEngine ce;

  ce.processOperation("d1");
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation( QString::fromStdString(ce.digitDot) );
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation("d2");
  QCOMPARE(ce.getScreenText(), QString("1,2") );

  ce.processOperation( QString::fromStdString(ce.opPlus) );
  QCOMPARE(ce.getScreenText(), QString("1,2") );

  ce.processOperation("d3");
  QCOMPARE(ce.getScreenText(), QString("3") );
  ce.processOperation("d4");
  QCOMPARE(ce.getScreenText(), QString("34") );

  ce.processOperation( QString::fromStdString(ce.opMinus) );
  QCOMPARE(ce.getScreenText(), QString("35,2") );

  ce.processOperation("d5");
  QCOMPARE(ce.getScreenText(), QString("5") );


  ce.processOperation( QString::fromStdString(ce.opResult) );
  QCOMPARE(ce.getScreenText(), QString("30,2") );
}

// === ========================================================================
// This is for issue #6
void TestCalculatorEngine::actionsScenario_SeqAddSubNoClear() {
  CalculatorEngine ce;

  ce.processOperation("d1");
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation( QString::fromStdString(ce.opPlus) );
  QCOMPARE(ce.getScreenText(), QString("1") );

  ce.processOperation("d5");
  QCOMPARE(ce.getScreenText(), QString("5") );

  ce.processOperation( QString::fromStdString(ce.opResult) );
  QCOMPARE(ce.getScreenText(), QString("6") );

  ce.processOperation("d8");
  QCOMPARE(ce.getScreenText(), QString("8") );

  ce.processOperation( QString::fromStdString(ce.opMinus) );
  QCOMPARE(ce.getScreenText(), QString("8") );

  ce.processOperation("d4");
  QCOMPARE(ce.getScreenText(), QString("4") );

  ce.processOperation( QString::fromStdString(ce.opResult) );
  QCOMPARE(ce.getScreenText(), QString("4") );

  ce.processOperation( QString::fromStdString(ce.opResult) );
  QCOMPARE(ce.getScreenText(), QString("0") );
}

// === ========================================================================
// This is for issue #9
void TestCalculatorEngine::actionsScenario_SeqAddResultSub() {
  CalculatorEngine ce;

  ce.processOperation("d1");
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation( QString::fromStdString(ce.opPlus) );
  QCOMPARE(ce.getScreenText(), QString("1") );

  ce.processOperation("d5");
  QCOMPARE(ce.getScreenText(), QString("5") );

  ce.processOperation( QString::fromStdString(ce.opResult) );
  QCOMPARE(ce.getScreenText(), QString("6") );

  ce.processOperation( QString::fromStdString(ce.opMinus) );
  QCOMPARE(ce.getScreenText(), QString("6") );

  ce.processOperation("d4");
  QCOMPARE(ce.getScreenText(), QString("4") );

  ce.processOperation( QString::fromStdString(ce.opResult) );
  QCOMPARE(ce.getScreenText(), QString("2") );
}

// === ========================================================================
// This scenarion sets memory and then clears it
// Note the value gets cleared after it was displayed
void TestCalculatorEngine::actionsScenario_MemSetClearA() {
  CalculatorEngine ce;
  QCOMPARE(ce.getMemoryState(), false);

  ce.processOperation( QString::fromStdString(ce.opMemoryUse) );
  QCOMPARE(ce.getScreenText(), QString("0") );

  ce.processOperation("d1");
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation( QString::fromStdString(ce.digitDot) );
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation("d2");
  QCOMPARE(ce.getScreenText(), QString("1,2") );
  QCOMPARE(ce.getMemoryState(), false);

  ce.processOperation( QString::fromStdString(ce.opMemoryPlus) );
  QCOMPARE(ce.getScreenText(), QString("1,2") );
  QCOMPARE(ce.getMemoryState(), true);

  ce.processOperation(QString::fromStdString(ce.opClearAll) );
  QCOMPARE(ce.getScreenText(), QString("0") );
  QCOMPARE(ce.getMemoryState(), true);

  ce.processOperation( QString::fromStdString(ce.opMemoryUse) );
  QCOMPARE(ce.getScreenText(), QString("1,2") );
  QCOMPARE(ce.getMemoryState(), true);

  ce.processOperation( QString::fromStdString(ce.opMemoryClear) );
  QCOMPARE(ce.getScreenText(), QString("1,2") );
  QCOMPARE(ce.getMemoryState(), false);

  ce.processOperation( QString::fromStdString(ce.opMemoryUse) );
  QCOMPARE(ce.getScreenText(), QString("0") );
}

// === ========================================================================
// This scenarion sets memory and then clears it
// Note the value gets cleared during some other operations
void TestCalculatorEngine::actionsScenario_MemSetClearB() {
  CalculatorEngine ce;
  QCOMPARE(ce.getMemoryState(), false);

  ce.processOperation( QString::fromStdString(ce.opMemoryUse) );
  QCOMPARE(ce.getScreenText(), QString("0") );

  ce.processOperation("d1");
  QCOMPARE(ce.getScreenText(), QString("1") );
  QCOMPARE(ce.getMemoryState(), false);

  ce.processOperation( QString::fromStdString(ce.opMemoryPlus) );
  QCOMPARE(ce.getScreenText(), QString("1") );
  QCOMPARE(ce.getMemoryState(), true);

  ce.processOperation(QString::fromStdString(ce.opClearAll) );
  QCOMPARE(ce.getScreenText(), QString("0") );
  QCOMPARE(ce.getMemoryState(), true);

  ce.processOperation("d5");
  QCOMPARE(ce.getScreenText(), QString("5") );
  ce.processOperation( QString::fromStdString(ce.opMult) );
  QCOMPARE(ce.getScreenText(), QString("5") );
  ce.processOperation("d4");
  QCOMPARE(ce.getScreenText(), QString("4") );

  QCOMPARE(ce.getMemoryState(), true);
  ce.processOperation( QString::fromStdString(ce.opMemoryClear) );
  QCOMPARE(ce.getScreenText(), QString("4") );
  QCOMPARE(ce.getMemoryState(), false);

  ce.processOperation( QString::fromStdString(ce.opResult) );
  QCOMPARE(ce.getScreenText(), QString("20") );
  QCOMPARE(ce.getMemoryState(), false);
}

// === ========================================================================

void TestCalculatorEngine::actionsScenario_MemSetUseA() {
  CalculatorEngine ce;

  ce.processOperation("d1");
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation( QString::fromStdString(ce.digitDot) );
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation("d2");
  QCOMPARE(ce.getScreenText(), QString("1,2") );
  QCOMPARE(ce.getMemoryState(), false);

  ce.processOperation( QString::fromStdString(ce.opMemoryPlus) );
  QCOMPARE(ce.getScreenText(), QString("1,2") );
  QCOMPARE(ce.getMemoryState(), true);

  ce.processOperation(QString::fromStdString(ce.opClearAll) );
  QCOMPARE(ce.getScreenText(), QString("0") );

  ce.processOperation( QString::fromStdString(ce.opMemoryUse) );
  QCOMPARE(ce.getScreenText(), QString("1,2") );
  QCOMPARE(ce.getMemoryState(), true);
}

// === ========================================================================
// This test for both issues #1 and #2
void TestCalculatorEngine::actionsScenario_MemSetUseB() {
  CalculatorEngine ce;

  ce.processOperation("d1");
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation( QString::fromStdString(ce.digitDot) );
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation("d0");
  QCOMPARE(ce.getScreenText(), QString("1,0") );
  ce.processOperation("d0");
  QCOMPARE(ce.getScreenText(), QString("1,00") );
  ce.processOperation("d1");
  QCOMPARE(ce.getScreenText(), QString("1,001") );

  ce.processOperation( QString::fromStdString(ce.opMemoryPlus) );
  QCOMPARE(ce.getScreenText(), QString("1,001") );

  ce.processOperation(QString::fromStdString(ce.opClearAll) );
  QCOMPARE(ce.getScreenText(), QString("0") );

  ce.processOperation( QString::fromStdString(ce.opMemoryUse) );
  QCOMPARE(ce.getScreenText(), QString("1,001") );
}

// === ========================================================================

void TestCalculatorEngine::actionsScenario_MemSetUseAdd() {
  CalculatorEngine ce;

  ce.processOperation("d1");
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation( QString::fromStdString(ce.digitDot) );
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation("d2");
  QCOMPARE(ce.getScreenText(), QString("1,2") );
  QCOMPARE(ce.getMemoryState(), false);

  ce.processOperation( QString::fromStdString(ce.opMemoryPlus) );
  QCOMPARE(ce.getScreenText(), QString("1,2") );
  QCOMPARE(ce.getMemoryState(), true);

  ce.processOperation(QString::fromStdString(ce.opClearAll) );
  QCOMPARE(ce.getScreenText(), QString("0") );

  ce.processOperation("d3");
  QCOMPARE(ce.getScreenText(), QString("3") );

  ce.processOperation( QString::fromStdString(ce.opPlus) );
  QCOMPARE(ce.getScreenText(), QString("3") );

  ce.processOperation( QString::fromStdString(ce.opMemoryUse) );
  QCOMPARE(ce.getScreenText(), QString("1,2") );


  ce.processOperation( QString::fromStdString(ce.opResult) );
  QCOMPARE(ce.getScreenText(), QString("4,2") );
}

// === ========================================================================

void TestCalculatorEngine::actionsScenario_MemSetUseSub() {
  CalculatorEngine ce;

  ce.processOperation("d1");
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation( QString::fromStdString(ce.digitDot) );
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation("d2");
  QCOMPARE(ce.getScreenText(), QString("1,2") );
  QCOMPARE(ce.getMemoryState(), false);

  ce.processOperation( QString::fromStdString(ce.opMemoryPlus) );
  QCOMPARE(ce.getScreenText(), QString("1,2") );
  QCOMPARE(ce.getMemoryState(), true);

  ce.processOperation(QString::fromStdString(ce.opClearAll) );
  QCOMPARE(ce.getScreenText(), QString("0") );

  ce.processOperation("d3");
  QCOMPARE(ce.getScreenText(), QString("3") );
  ce.processOperation("d4");
  QCOMPARE(ce.getScreenText(), QString("34") );

  ce.processOperation( QString::fromStdString(ce.opMinus) );
  QCOMPARE(ce.getScreenText(), QString("34") );

  ce.processOperation(QString::fromStdString(ce.opMemoryUse));
  QCOMPARE(ce.getScreenText(), QString("1,2") );


  ce.processOperation( QString::fromStdString(ce.opResult) );
  QCOMPARE(ce.getScreenText(), QString("32,8") );
}

// === ========================================================================
// See also Issue #1
void TestCalculatorEngine::actionsScenario_MemUseBadFloatA(){
  CalculatorEngine ce;

  ce.processOperation("d1");
  QCOMPARE(ce.getScreenText(), QString("1") );
  ce.processOperation("d2");
  QCOMPARE(ce.getScreenText(), QString("12") );
  ce.processOperation("d3");
  QCOMPARE(ce.getScreenText(), QString("123") );
  ce.processOperation("d4");
  QCOMPARE(ce.getScreenText(), QString("1234") );
  ce.processOperation("d5");
  QCOMPARE(ce.getScreenText(), QString("12345") );
  ce.processOperation("d6");
  QCOMPARE(ce.getScreenText(), QString("123456") );
  ce.processOperation("d7");
  QCOMPARE(ce.getScreenText(), QString("1234567") );

  ce.processOperation( QString::fromStdString(ce.digitDot) );
  QCOMPARE(ce.getScreenText(), QString("1234567") );

  ce.processOperation("d8");
  QCOMPARE(ce.getScreenText(), QString("1234567,8") );
  ce.processOperation("d9");
  QCOMPARE(ce.getScreenText(), QString("1234567,89") );
  QCOMPARE(ce.getMemoryState(), false);

  ce.processOperation( QString::fromStdString(ce.opMemoryPlus) );
  QCOMPARE(ce.getScreenText(), QString("1234567,89") );
  QCOMPARE(ce.getMemoryState(), true);

  ce.processOperation(QString::fromStdString(ce.opClearAll) );
  QCOMPARE(ce.getScreenText(), QString("0") );

  ce.processOperation(QString::fromStdString(ce.opMemoryUse));
  QCOMPARE(ce.getScreenText(), QString("1234567,89") );
}

// === ========================================================================
// This is for Issue #8
void TestCalculatorEngine::actionsScenario_MemSetAfterResultA(){
  CalculatorEngine ce;

  ce.processOperation("d1");
  QCOMPARE(ce.getScreenText(), QString("1") );

  ce.processOperation( QString::fromStdString(ce.opPlus) );
  QCOMPARE(ce.getScreenText(), QString("1") );

  ce.processOperation("d3");
  QCOMPARE(ce.getScreenText(), QString("3") );

  ce.processOperation( QString::fromStdString(ce.opResult) );
  QCOMPARE(ce.getScreenText(), QString("4") );
  QCOMPARE(ce.getMemoryState(), false);

  ce.processOperation( QString::fromStdString(ce.opMemoryPlus) );
  QCOMPARE(ce.getScreenText(), QString("4") );
  QCOMPARE(ce.getMemoryState(), true);

  ce.processOperation(QString::fromStdString(ce.opClearAll) );
  QCOMPARE(ce.getScreenText(), QString("0") );
  QCOMPARE(ce.getMemoryState(), true);

  ce.processOperation(QString::fromStdString(ce.opMemoryUse));
  QCOMPARE(ce.getScreenText(), QString("4") );
  QCOMPARE(ce.getMemoryState(), true);
}

// === ========================================================================
// This is for Issue #8
void TestCalculatorEngine::actionsScenario_MemSetAfterResultB(){
  CalculatorEngine ce;

  ce.processOperation("d1");
  QCOMPARE(ce.getScreenText(), QString("1") );

  ce.processOperation( QString::fromStdString(ce.opMinus) );
  QCOMPARE(ce.getScreenText(), QString("1") );

  ce.processOperation("d8");
  QCOMPARE(ce.getScreenText(), QString("8") );

  ce.processOperation( QString::fromStdString(ce.opResult) );
  QCOMPARE(ce.getScreenText(), QString("-7") );
  QCOMPARE(ce.getMemoryState(), false);

  ce.processOperation( QString::fromStdString(ce.opMemoryPlus) );
  QCOMPARE(ce.getScreenText(), QString("-7") );
  QCOMPARE(ce.getMemoryState(), true);

  ce.processOperation(QString::fromStdString(ce.opClearAll) );
  QCOMPARE(ce.getScreenText(), QString("0") );
  QCOMPARE(ce.getMemoryState(), true);

  ce.processOperation(QString::fromStdString(ce.opMemoryUse));
  QCOMPARE(ce.getScreenText(), QString("-7") );
  QCOMPARE(ce.getMemoryState(), true);

  ce.processOperation("d7");
  QCOMPARE(ce.getScreenText(), QString("7") );
  QCOMPARE(ce.getMemoryState(), true);

  ce.processOperation( QString::fromStdString(ce.opMemoryPlus) );
  QCOMPARE(ce.getScreenText(), QString("7") );
  QCOMPARE(ce.getMemoryState(), false);

}

// === ========================================================================
// This is for Issue #8
void TestCalculatorEngine::actionsScenario_MemSetAfterAction(){
  CalculatorEngine ce;

  ce.processOperation("d1");
  QCOMPARE(ce.getScreenText(), QString("1") );

  ce.processOperation( QString::fromStdString(ce.opPlus) );
  QCOMPARE(ce.getScreenText(), QString("1") );

  ce.processOperation("d3");
  QCOMPARE(ce.getScreenText(), QString("3") );

  ce.processOperation( QString::fromStdString(ce.opMinus) );
  QCOMPARE(ce.getScreenText(), QString("4") );
  QCOMPARE(ce.getMemoryState(), false);

  ce.processOperation( QString::fromStdString(ce.opMemoryPlus) );
  QCOMPARE(ce.getScreenText(), QString("4") );
  QCOMPARE(ce.getMemoryState(), true);

  ce.processOperation(QString::fromStdString(ce.opClearAll) );
  QCOMPARE(ce.getScreenText(), QString("0") );
  QCOMPARE(ce.getMemoryState(), true);

  ce.processOperation(QString::fromStdString(ce.opMemoryUse));
  QCOMPARE(ce.getScreenText(), QString("4") );
  QCOMPARE(ce.getMemoryState(), true);
}

// === ========================================================================
//
void TestCalculatorEngine::actionsScenario_MemUseMemPlusA(){
  CalculatorEngine ce;
  QCOMPARE(ce.getMemoryState(), false);

  ce.processOperation("d1");
  QCOMPARE(ce.getScreenText(), QString("1") );

  ce.processOperation( QString::fromStdString(ce.opMemoryPlus) );
  QCOMPARE(ce.getScreenText(), QString("1") );
  QCOMPARE(ce.getMemoryState(), true);

  ce.processOperation(QString::fromStdString(ce.opClearAll) );
  QCOMPARE(ce.getScreenText(), QString("0") );
  QCOMPARE(ce.getMemoryState(), true);

  ce.processOperation(QString::fromStdString(ce.opMemoryUse));
  QCOMPARE(ce.getScreenText(), QString("1") );

  ce.processOperation( QString::fromStdString(ce.opMemoryPlus) );
  QCOMPARE(ce.getScreenText(), QString("1") );

  ce.processOperation(QString::fromStdString(ce.opMemoryUse));
  QCOMPARE(ce.getScreenText(), QString("2") );
  QCOMPARE(ce.getMemoryState(), true);
}

// === ========================================================================
//
void TestCalculatorEngine::actionsScenario_MemUseMemPlusB(){
  CalculatorEngine ce;
  QCOMPARE(ce.getMemoryState(), false);

  ce.processOperation("d1");
  QCOMPARE(ce.getScreenText(), QString("1") );

  ce.processOperation( QString::fromStdString(ce.opPlus) );
  QCOMPARE(ce.getScreenText(), QString("1") );

  ce.processOperation("d3");
  QCOMPARE(ce.getScreenText(), QString("3") );
  QCOMPARE(ce.getMemoryState(), false);

  ce.processOperation( QString::fromStdString(ce.opMemoryPlus) );
  QCOMPARE(ce.getScreenText(), QString("3") );
  QCOMPARE(ce.getMemoryState(), true);

  ce.processOperation( QString::fromStdString(ce.opMinus) );
  QCOMPARE(ce.getScreenText(), QString("4") );
  QCOMPARE(ce.getMemoryState(), true);

  ce.processOperation(QString::fromStdString(ce.opClearAll) );
  QCOMPARE(ce.getScreenText(), QString("0") );
  QCOMPARE(ce.getMemoryState(), true);

  ce.processOperation(QString::fromStdString(ce.opMemoryUse));
  QCOMPARE(ce.getScreenText(), QString("3") );
  QCOMPARE(ce.getMemoryState(), true);
}

// === ========================================================================

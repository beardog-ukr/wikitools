#include "TestCalculatorEngine.h"

// === =======================================================================

int main(int , char **)
{
  QStringList testCmd;
  TestCalculatorEngine testCalculatorEngine;
  //QDir testLogDir;
  //testLogDir.mkdir("UnitTest_Results");
  //testCmd<<" "<<"-o" <<"UnitTest_Results/test_log.txt";
  QTest::qExec(&testCalculatorEngine,testCmd);

  return 0;
}

#include "TestEngine.h"

int main(int argc, char **argv)
{
  TestEngine testEngine;
  QTest::qExec(&testEngine, argc, argv);

  return 0;
}

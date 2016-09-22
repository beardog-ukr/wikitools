#include "TestBasic.h"

int main(int argc, char **argv)
{
  TestBasic testBasic;
  QTest::qExec(&testBasic, argc, argv);

  return 0;
}

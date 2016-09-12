#include "TestBasicHttpReader.h"
#include "TestSimplePageReader.h"
#include "TestWikiCategoryReader.h"

int main(int , char **)
{
  QStringList testCmd;

  TestBasicHttpReader testBasicHttpReader;
  QTest::qExec(&testBasicHttpReader,testCmd);
  
  TestSimplePageReader testSimplePageReader;
  QTest::qExec(&testSimplePageReader,testCmd);

  TestWikiCategoryReader testWikiCategoryReader;
  QTest::qExec(&testWikiCategoryReader,testCmd);

  
  return 0;
}

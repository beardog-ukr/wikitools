#include "ActionBasic.h"

//=== ========================================================================

ActionBasic::ActionBasic(QObject* parent)
            :QObject(parent) {
  c5 = 0;
  errorMessage = "";
}

//=== ========================================================================

ActionBasic::~ActionBasic() {
  //ntdh
}

//=== ========================================================================

QString ActionBasic::getErrorMessage() const {
  return errorMessage;
}

//=== ========================================================================

void ActionBasic::setLogger(FiveCatsLogger* c5p) {
  c5 = c5p ;
}

//=== ========================================================================


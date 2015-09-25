#include "Semigroup.h"

namespace omalg {

  Semigroup::Semigroup(std::vector<std::string> theElementNames, std::vector<std::vector<size_t> > theMultiplicationTable)
    : elementNames(theElementNames), multiplicationTable(theMultiplicationTable) {
    this->hClasses = 0;
    this->rClasses = 0;
    this->lClasses = 0;
    this->jClasses = 0;
    this->jOrder = 0;
  }

  inline size_t Semigroup::product(const size_t lhs, const size_t rhs) const {
    return this->multiplicationTable[lhs][rhs];
  }
}

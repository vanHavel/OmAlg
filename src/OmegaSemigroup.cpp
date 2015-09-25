#include "OmegaSemigroup.h"

namespace omalg {
  OmegaSemigroup::OmegaSemigroup(Semigroup theSemigroup, std::vector<std::string> theOmegaElementNames,
      std::vector<std::vector<size_t> > theMixedProductTable, std::vector<size_t> theOmegaIterationTable,
      std::vector<bool> theP, Morphism thePhi)
        : sPlus(theSemigroup), omegaElementNames(theOmegaElementNames),
          mixedProductTable(theMixedProductTable), omegaIterationTable(theOmegaIterationTable),
          P(theP), phi(thePhi) { }

  inline size_t OmegaSemigroup::product(size_t lhs, size_t rhs) const {
    return this->sPlus.product(lhs, rhs);
  }

  inline size_t OmegaSemigroup::mixedProduct(size_t plusElement, size_t omegaElement) const {
    return this->mixedProductTable[plusElement][omegaElement];
  }

  inline size_t OmegaSemigroup::omegaIteration(size_t element) const {
    return this->omegaIterationTable[element];
  }


}

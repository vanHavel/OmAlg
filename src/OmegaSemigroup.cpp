#include "OmegaSemigroup.h"

namespace omalg {
  OmegaSemigroup::OmegaSemigroup(Semigroup theSemigroup, std::vector<std::string> theOmegaElementNames,
      std::vector<std::vector<size_t> > theMixedProductTable, std::vector<size_t> theOmegaIterationTable,
      std::vector<bool> theP, Morphism thePhi)
        : sPlus(theSemigroup), omegaElementNames(theOmegaElementNames),
          mixedProductTable(theMixedProductTable), omegaIterationTable(theOmegaIterationTable),
          P(theP), phi(thePhi) { }


}

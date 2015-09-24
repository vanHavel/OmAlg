#ifndef OMALG_OMEGA_SEMIGROUP
#define OMALG_OMEGA_SEMIGROUP

#include <vector>
#include <string>

#include "Semigroup.h"
#include "Morphism.h"

namespace omalg {

  class OmegaSemigroup {
  private:
  	Semigroup sPlus;
  	std::vector<std::string> omegaElementNames;
  	std::vector<std::vector<size_t> > mixedProductTable;
  	std::vector<size_t> omegaIterationTable;
  	std::vector<bool> P;
  	Morphism phi;
  public:
  	/**
  	 * Omega Semigroup Constructor.
  	 * @param theSemigroup Semigroup part of the omega semigroup.
  	 * @param theOmegaElementNames Names of the elements of S_omega.
  	 * @param theMixedProductTable Table for the mixed product.
  	 * @param theOmegaIterationTable Table for the omega iteration operation.
  	 */
  	OmegaSemigroup(Semigroup theSemigroup,                                 std::vector<std::string> theOmegaElementNames,
  	               std::vector<std::vector<size_t> > theMixedProductTable, std::vector<size_t> theOmegaIterationTable,
  	               std::vector<bool> theP,                                 Morphism thePhi);

  };

}

#endif

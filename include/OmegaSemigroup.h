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
  	/**
  	 * Finite semigroup product(No check of bounds).
  	 * @param lhs The left factor.
  	 * @param rhs The right factor.
  	 * @return Index of product.
  	 */
  	size_t product(size_t lhs, size_t rhs) const;
  	/**
  	 * Mixed product of sPlus and sOmega(no check of bounds),
  	 * @param plusElement Element of sPlus.
  	 * @param omegaElement Element of sOmega.
  	 * @return Index of mixed product result.
  	 */
  	size_t mixedProduct(size_t plusElement, size_t omegaElement) const;
  	/**
  	 * Omega iteration operation.
  	 * @param element Element of sPlus.
  	 * @return Index of iteration result.
  	 */
  	size_t omegaIteration(size_t element) const;
    /**
     * Returns textual description of omega semigroup in OmAlg format.
     * @return The description as a string.
     */
    std::string description() const;
  };


}

#endif

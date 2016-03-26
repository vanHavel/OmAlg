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

  	void invertP();

  public:
  	/**
  	 * Omega Semigroup Constructor.
  	 * @param theSemigroup Semigroup part of the omega semigroup.
  	 * @param theOmegaElementNames Names of the elements of S_omega.
  	 * @param theMixedProductTable Table for the mixed product.
  	 * @param theOmegaIterationTable Table for the omega iteration operation.
  	 * @param theP Set of "accepting" elements P, represented by bit vector
  	 * @param thePhi Morphism from a finite alphabet into the omega semigroup
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

    /**
     * Checks whether the recognized language can be accepted by a deterministic Büchi automaton.
     * @return True iff DBA-recognizable
     */
    bool isDBRecognizable() const;

    /**
     * Checks whether the recognized language can be accepted by a coBüchi automaton.
     * @return True iff DCA-recognizable
     */
    bool isDCRecognizable() const;

    /**
     * Checks whether the recognized language can be accepted by a deterministic E automaton.
     * @return True iff E-recognizable
     */
    bool isERecognizable() const;

    /**
     * Checks whether the recognized language can be accepted by an A-automaton.
     * @return True iff A-recognizable
     */
    bool isARecognizable() const;

    /**
     * Checks whether the recognized language can be accepted by a deterministic weak Büchi automaton.
     * @return True iff WB-recognizable
     */
    bool isWBRecognizable() const;
  };


}

#endif

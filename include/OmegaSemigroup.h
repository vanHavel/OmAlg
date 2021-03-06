#ifndef OMALG_OMEGA_SEMIGROUP
#define OMALG_OMEGA_SEMIGROUP

#include <vector>
#include <string>

#include "Semigroup.h"
#include "Morphism.h"
#include "GeneralException.h"

namespace omalg {

  class DeterministicBuechiAutomaton;
  class DeterministicCoBuechiAutomaton;
	class DeterministicParityAutomaton;

  class OmegaSemigroup {

		//The automaton converter can access private fields of the omega semigroup.
		friend class AutomatonConverter;

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
		 * Reduces to syntactic omega semigroup. This is very expensive(O(n^5)).
		 */
    void reduceToSyntactic();

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

		/**
		 * Turns omega semigroup into equivalent weak Buechi automaton, if possible.
		 * @return Equivalent weak Buechi automaton
		 * @throw OperationNotApplicableException if recognized language is not weak Buechi recognizable
		 */
    DeterministicBuechiAutomaton toWeakBuechi() const;

		/**
		 * Turns omega semigroup into equivalent coBuechi automaton, if possible.
		 * @return Equivalent coBuechi automaton
		 * @throw OperationNotApplicableException if recognized language is not coBuechi recognizable
		 */
		DeterministicCoBuechiAutomaton toCoBuechi() const;

    /**
		 * Turns omega semigroup into equivalent det. Buechi automaton, if possible.
		 * @return Equivalent det. Buechi automaton
		 * @throw OperationNotApplicableException if recognized language is not det. Buechi recognizable
		 */
    DeterministicBuechiAutomaton toDetBuechi() const;

    /**
		 * Turns omega semigroup into equivalent deterministic parity automaton.
		 * @return Equivalent parity automaton
		 */
    DeterministicParityAutomaton toParity() const;

  private:
    /**
     * Check whether two elements of sPlus are syntactically equivalent.
     * @param lhs the first element.
     * @param rhs the second element.
     * @return true iff the elements are equivalent.
     */
    bool sPlusEquivalent(size_t lhs, size_t rhs) const;

    /**
     * Check whether two elements of sOmega are syntactically equivalent.
     * @param lhs the first element.
     * @param rhs the second element.
     * @return true iff the elements are equivalent.
     */
    bool sOmegaEquivalent(size_t lhs, size_t rhs) const;
  };


}

#endif

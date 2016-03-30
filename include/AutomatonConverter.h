#ifndef OMALG_PARITY_AUTOMATON_CONVERTER
#define OMALG_PARITY_AUTOMATON_CONVERTER

#include <list>
#include <string>

#include "OmegaSemigroup.h"
#include "DeterministicParityAutomaton.h"
#include "DeterministicCoBuechiAutomaton.h"
#include "DeterministicBuechiAutomaton.h"

namespace omalg {

  /**
   * Utility class to transform given omega semigroup into equivalent automata.
   */
  class AutomatonConverter {
  public:
    /**
     * Constructor.
     * @param S the input omega semigroup.
     */
    AutomatonConverter(OmegaSemigroup const &theS);

    /**
     * Execute conversion to parity automaton.
     * @return The resulting parity automaton.
     */
    DeterministicParityAutomaton convertToParity() const;

    /**
     * Execute conversion to coBuechi automaton.
     * @return The resulting coBuechi automaton.
     */
    DeterministicCoBuechiAutomaton convertToCoBuechi() const;

    /**
     * Execute conversion to weak Buechi automaton.
     * @return The resulting weak Buechi automaton.
     */
    DeterministicBuechiAutomaton convertToWeakBuechi() const;
  private:
    OmegaSemigroup S;
    /**
     * Turns list of integers into a string as key for use in maps.
     */
    std::string stringValue(std::list<size_t> const& L) const;

    /**
     * Calculate successor state in parity automaton given current state and new rightmost element.
     * @param currentState list representation of the current state.
     * @param element new rightmost element.
     * @return list representation of successor state.
     */
    std::list<size_t> successorState(std::list<size_t> currentState, size_t element) const;

    /**
     * Calculate priority of a given state.
     * @param state list representation of the state.
     * @return the priority of the state.
     */
    size_t calculatePriority(std::list<size_t> const &state) const;

    /**
     * Assign name to a state.
     * @param state list representation of the state.
     * @return a string representation of the state.
     */
    std::string stateName(std::list<size_t> const &state) const;
  };

}

#endif

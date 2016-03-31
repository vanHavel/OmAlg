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
     * Execute conversion to det. Buechi automaton.
     * @return The resulting det. Buechi automaton.
     */
    DeterministicBuechiAutomaton convertToDetBuechi() const;

    /**
     * Execute conversion to weak Buechi automaton.
     * @return The resulting weak Buechi automaton.
     */
    DeterministicBuechiAutomaton convertToWeakBuechi() const;
  private:
    /**
     * The omega semigroup to transform. No method changes the state of the omega semigroup.
     * It is declared mutable to implement the conversion to det. Buechi more efficiently,
     * relying on the coBuechi conversion of the complement.
     */
    mutable OmegaSemigroup S;
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

    /**
     * Calculate successor state in coBuechi automaton given current state and new rightmost element.
     * @param currentState list representation of the current state.
     * @param element new rightmost element.
     * @return list representation of successor state.
     */
    std::list<size_t> CBAsuccessorState(std::list<size_t> const& currentState, size_t element) const;

    /**
     * Decide whether a given coBuechi state should be final.
     * @param state list representation of the state.
     * @return true iff the state is final.
     */
    bool decideFinality(std::list<size_t> const &state) const;

    /**
     * Decide whether a given element is sub loop accepting.
     * @param t the element to consider.
     * @return true iff t is sub loop accepting.
     */
    bool subLoopAccepting(size_t t) const;

    /**
     * Decide whether a given pair is sub loop accepting.
     * @param s first element of the pair.
     * @param t second element of the pair
     * @return true iff (s,t) is sub loop accepting.
     */
    bool subLoopAccepting(size_t s, size_t t) const;
  };

}

#endif

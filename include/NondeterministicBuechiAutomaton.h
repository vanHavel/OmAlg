#ifndef OMALG_NONDETERMINISTIC_BUECHI_AUTOMATON
#define OMALG_NONDETERMINISTIC_BUECHI_AUTOMATON

#include <vector>
#include <string>

#include "BuechiAutomaton.h"
#include "NondeterministicOmegaAutomaton.h"

namespace omalg {

  class NondeterministicBuechiAutomaton: public BuechiAutomaton, public NondeterministicOmegaAutomaton {
  public:
    NondeterministicBuechiAutomaton (int theNumberOfStates, std::vector<std::string> theStateNames,
                                     int theAlphabetSize,   std::vector<std::string> theAlphabet,
                                     int theInitialState,   std::vector<std::set<int> > theTransitionRelation,
                                     std::vector<bool>  theFinalStates);
    OmegaSemigroup* toOmegaSemigroup() const override;
  };
}

#endif

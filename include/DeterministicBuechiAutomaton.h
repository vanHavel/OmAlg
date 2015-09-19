#ifndef OMALG_DETERMINISTIC_BUECHI_AUTOMATON
#define OMALG_DETERMINISTIC_BUECHI_AUTOMATON

#include <vector>
#include <string>

#include "BuechiAutomaton.h"
#include "DeterministicOmegaAutomaton.h"

namespace omalg {

  class DeterministicBuechiAutomaton: public BuechiAutomaton, public DeterministicOmegaAutomaton {
  public:
    DeterministicBuechiAutomaton (int theNumberOfStates, std::vector<std::string> theStateNames,
                                  int theAlphabetSize,   std::vector<std::string> theAlphabet,
                                  int theInitialState,   std::vector<std::vector<int> > theTransitionTable,
                                  std::vector<bool>  theFinalStates);
    OmegaSemigroup* toOmegaSemigroup() const override;
  };
}

#endif

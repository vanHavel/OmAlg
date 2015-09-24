#ifndef OMALG_DETERMINISTIC_BUECHI_AUTOMATON
#define OMALG_DETERMINISTIC_BUECHI_AUTOMATON

#include <vector>
#include <string>

#include "BuechiAutomaton.h"
#include "DeterministicOmegaAutomaton.h"

namespace omalg {

  class DeterministicBuechiAutomaton: public BuechiAutomaton, public DeterministicOmegaAutomaton {
  public:
    DeterministicBuechiAutomaton (std::vector<std::string> theStateNames,
                                  std::vector<std::string> theAlphabet,
                                  size_t theInitialState,
                                  std::vector<std::vector<size_t> > theTransitionTable,
                                  std::vector<bool>  theFinalStates);
    OmegaSemigroup* toOmegaSemigroup() const override;
    std::string description() const override;
  };
}

#endif

#ifndef OMALG_DETERMINISTIC_PARITY_AUTOMATON
#define OMALG_DETERMINISTIC_PARITY_AUTOMATON

#include <vector>
#include <string>

#include "ParityAutomaton.h"
#include "DeterministicOmegaAutomaton.h"

namespace omalg {

  class DeterministicParityAutomaton: public ParityAutomaton, public DeterministicOmegaAutomaton {
  public:
    DeterministicParityAutomaton (std::vector<std::string> theStateNames,
                                  std::vector<std::string> theAlphabet,
                                  size_t theInitialState,
                                  std::vector<std::vector<size_t> > theTransitionTable,
                                  std::vector<size_t>  thePriorities);
    std::string description() const override;
    
    OmegaSemigroup* toOmegaSemigroup() const override;
    
    TransitionProfile<DeterministicParityAutomaton> getTransitionProfileForLetter(size_t index) const;
    TransitionProfile<DeterministicParityAutomaton> getEpsilonProfile() const;
  };
}

#include "DPTransitionProfile.h"

#endif

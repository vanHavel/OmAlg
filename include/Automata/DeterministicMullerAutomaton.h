#ifndef OMALG_DETERMINISTIC_MULLER_AUTOMATON
#define OMALG_DETERMINISTIC_MULLER_AUTOMATON

#include <vector>
#include <string>

#include "MullerAutomaton.h"
#include "DeterministicOmegaAutomaton.h"

namespace omalg {

  class DeterministicMullerAutomaton: public MullerAutomaton, public DeterministicOmegaAutomaton {
  public:
    DeterministicMullerAutomaton (std::vector<std::string> theStateNames,
                                  std::vector<std::string> theAlphabet,
                                  size_t theInitialState,
                                  std::vector<std::vector<size_t> > theTransitionTable,
                                  std::set<std::set<size_t> >  theTable);
    std::string description() const override;
    
    OmegaSemigroup* toOmegaSemigroup() const override;
    
    TransitionProfile<DeterministicMullerAutomaton> getTransitionProfileForLetter(size_t index) const;
    TransitionProfile<DeterministicMullerAutomaton> getEpsilonProfile() const;
  };
}

#include "DMTransitionProfile.h"

#endif

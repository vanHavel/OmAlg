#ifndef OMALG_NONDETERMINISTIC_PARITY_AUTOMATON
#define OMALG_NONDETERMINISTIC_PARITY_AUTOMATON

#include <vector>
#include <string>

#include "ParityAutomaton.h"
#include "NondeterministicOmegaAutomaton.h"

namespace omalg {

  class NondeterministicParityAutomaton: public ParityAutomaton, public NondeterministicOmegaAutomaton {
  public:
    NondeterministicParityAutomaton (std::vector<std::string> theStateNames,
                                     std::vector<std::string> theAlphabet,
                                     size_t theInitialState,
                                     std::vector<std::vector<std::set<size_t> > > theTransitionRelation,
                                     std::vector<size_t>  thePriorities);

    std::string description() const override;
    OmegaSemigroup* toOmegaSemigroup() const override;
    TransitionProfile<NondeterministicParityAutomaton> getTransitionProfileForLetter(size_t index) const;
    TransitionProfile<NondeterministicParityAutomaton> getEpsilonProfile() const;
  };
  
}

#include "NPTransitionProfile.h"

#endif

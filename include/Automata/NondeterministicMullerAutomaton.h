#ifndef OMALG_NONDETERMINISTIC_MULLER_AUTOMATON
#define OMALG_NONDETERMINISTIC_MULLER_AUTOMATON

#include <vector>
#include <string>

#include "MullerAutomaton.h"
#include "NondeterministicOmegaAutomaton.h"

namespace omalg {

  class NondeterministicMullerAutomaton: public MullerAutomaton, public NondeterministicOmegaAutomaton {
  public:
    NondeterministicMullerAutomaton (std::vector<std::string> theStateNames,
                                     std::vector<std::string> theAlphabet,
                                     size_t theInitialState,
                                     std::vector<std::vector<std::set<size_t> > > theTransitionRelation,
                                     std::set<std::set<size_t> > theTable);

    std::string description() const override;
    OmegaSemigroup* toOmegaSemigroup() const override;
    TransitionProfile<NondeterministicMullerAutomaton> getTransitionProfileForLetter(size_t letter) const;
    TransitionProfile<NondeterministicMullerAutomaton> getEpsilonProfile() const;
  };
  
}

#include "NMTransitionProfile.h"

#endif

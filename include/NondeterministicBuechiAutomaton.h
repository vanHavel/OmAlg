#ifndef OMALG_NONDETERMINISTIC_BUECHI_AUTOMATON
#define OMALG_NONDETERMINISTIC_BUECHI_AUTOMATON

#include <vector>
#include <string>

#include "BuechiAutomaton.h"
#include "NondeterministicOmegaAutomaton.h"

namespace omalg {

  class NondeterministicBuechiAutomaton: public BuechiAutomaton, public NondeterministicOmegaAutomaton {
  public:
    NondeterministicBuechiAutomaton (std::vector<std::string> theStateNames,
                                     std::vector<std::string> theAlphabet,
                                     size_t theInitialState,
                                     std::vector<std::vector<std::set<size_t> > > theTransitionRelation,
                                     std::vector<bool>  theFinalStates);

    std::string description() const override;
    OmegaSemigroup* toOmegaSemigroup() const override;
    TransitionProfile<NondeterministicBuechiAutomaton> getTransitionProfileForLetter(size_t index) const;
  };
  
}

#include "NBTransitionProfile.h"

#endif

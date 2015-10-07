#ifndef NONDETERMINISTIC_COBUECHI_AUTOMATON
#define	NONDETERMINISTIC_COBUECHI_AUTOMATON

#include <vector>
#include <string>

#include "CoBuechiAutomaton.h"
#include "NondeterministicOmegaAutomaton.h"

namespace omalg {

  class NondeterministicCoBuechiAutomaton: public CoBuechiAutomaton, public NondeterministicOmegaAutomaton {
  public:
    NondeterministicCoBuechiAutomaton (std::vector<std::string> theStateNames,
                                       std::vector<std::string> theAlphabet,
                                       size_t theInitialState,
                                       std::vector<std::vector<std::set<size_t> > > theTransitionRelation,
                                       std::vector<bool>  theFinalStates);

    std::string description() const override;
    OmegaSemigroup* toOmegaSemigroup() const override;
    TransitionProfile<NondeterministicCoBuechiAutomaton> getTransitionProfileForLetter(size_t index) const;
    TransitionProfile<NondeterministicCoBuechiAutomaton> getEpsilonProfile() const;
  };
  
}

#include "NCTransitionProfile.h"

#endif	
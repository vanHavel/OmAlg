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

    OmegaSemigroup* toOmegaSemigroup() const override;

    std::string description() const override;
  };
}

#endif

#ifndef OMALG_DETERMINISTIC_COBUECHI_AUTOMATON
#define OMALG_DETERMINISTIC_COBUECHI_AUTOMATON

#include <vector>
#include <string>

#include "CoBuechiAutomaton.h"
#include "DeterministicOmegaAutomaton.h"

class DeterministicBuechiAutomaton;

namespace omalg {

  class DeterministicCoBuechiAutomaton: public CoBuechiAutomaton, public DeterministicOmegaAutomaton {
  public:
    DeterministicCoBuechiAutomaton (std::vector<std::string> theStateNames,
                                    std::vector<std::string> theAlphabet,
                                    size_t theInitialState,
                                    std::vector<std::vector<size_t> > theTransitionTable,
                                    std::vector<bool>  theFinalStates);
    std::string description() const override;
    
    OmegaSemigroup* toOmegaSemigroup() const override;
    
    TransitionProfile<DeterministicCoBuechiAutomaton> getTransitionProfileForLetter(size_t index) const;
    TransitionProfile<DeterministicCoBuechiAutomaton> getEpsilonProfile() const;

    /**
     * Constructs the dual automaton - a det. Buechi automaton for the complement.
     * @return the dual automaton.
     */
    DeterministicBuechiAutomaton dual() const;
  };
}

#include "DCTransitionProfile.h"

#endif

#ifndef OMALG_DETERMINISTIC_OMEGA_AUTOMATON
#define OMALG_DETERMINISTIC_OMEGA_AUTOMATON

#include "OmegaAutomaton.h"
typedef std::vector<std::vector<int> > TransitionTable; 

namespace omalg {

  class DeterministicOmegaAutomaton: public OmegaAutomaton {
  private:
    TransitionTable transitionTable;
  
  };

}
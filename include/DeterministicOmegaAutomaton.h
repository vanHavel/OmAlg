#ifndef OMALG_DETERMINISTIC_OMEGA_AUTOMATON
#define OMALG_DETERMINISTIC_OMEGA_AUTOMATON

#include <vector>

#include "OmegaAutomaton.h"

namespace omalg {

  class DeterministicOmegaAutomaton: public virtual OmegaAutomaton {
	private:
		std::vector<std::vector<int> > transitionTable;
		  
	protected:
		DeterministicOmegaAutomaton(std::vector<std::vector<int> > theTransitionTable);
  };
}

#endif

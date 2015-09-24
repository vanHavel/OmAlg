#ifndef OMALG_DETERMINISTIC_OMEGA_AUTOMATON
#define OMALG_DETERMINISTIC_OMEGA_AUTOMATON

#include <vector>

#include "OmegaAutomaton.h"

namespace omalg {

  class DeterministicOmegaAutomaton: public virtual OmegaAutomaton {
	private:
		std::vector<std::vector<size_t> > transitionTable;
		  
	protected:
		DeterministicOmegaAutomaton(std::vector<std::vector<size_t> > theTransitionTable);

	public:
		virtual std::string description() const override;
  };
}

#endif

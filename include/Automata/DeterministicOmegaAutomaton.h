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
		std::vector<std::vector<size_t> > getTransitionTable() const;

	public:
	  virtual std::string description() const override;
      /**
       * Get target state for given state and letter.
       * @param state Index of origin state.
       * @param letter Index of alphabet letter.
       * @return The unique target state.
       */
      size_t getTarget(size_t state, size_t letter) const;
  };
}

#endif

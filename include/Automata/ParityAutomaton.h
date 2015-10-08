#ifndef OMALG_PARITY_AUTOMATON
#define OMALG_PARITY_AUTOMATON

#include <vector>

#include "OmegaAutomaton.h"

namespace omalg {

  class ParityAutomaton: public virtual OmegaAutomaton {
  private:
    std::vector<size_t> priorities;

  protected:
    ParityAutomaton(std::vector<size_t> thePriorities);
  public:
    virtual std::string description() const override;
    /**
     * Returns priority for given state.
     * @param state Index of the state to consider.
     * @return the priority of the state.
     */
    size_t priority(size_t state) const;
  };
}

#endif

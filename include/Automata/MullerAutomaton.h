#ifndef OMALG_MULLER_AUTOMATON
#define OMALG_MULLER_AUTOMATON

#include <set>

#include "OmegaAutomaton.h"

namespace omalg {

  class MullerAutomaton: public virtual OmegaAutomaton {
  private:
    std::set<std::set<size_t> > table;
  protected:
    MullerAutomaton(std::set<std::set<size_t> > theTable);
  public:
    virtual std::string description() const override;
    /**
     * Returns whether a given state set is in the table.
     * @param stateSet The state set to consider.
     * @return true iff the set is in the table.
     */
    bool inTable(std::set<size_t> stateSet) const;
  };
}

#endif

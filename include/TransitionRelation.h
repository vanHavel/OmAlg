#ifndef OMALG_TRANSITION_RELATION
#define OMALG_TRANSITION_RELATION

#include <vector>
#include <set>

namespace omalg {

  class TransitionRelation {
  private:
    std::vector<std::vector<std::set<int> > > TransitionTable;
    
  public:
    std::set<int> getTargets(int state, int letter);
    std::vector<std::set<int> > getSuccessors(int state);
  };

}

#endif
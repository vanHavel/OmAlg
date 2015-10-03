#ifndef OMALG_NBTRANSITION_PROFILE
#define	OMALG_NBTRANSITION_PROFILE

#include <vector>
#include <set>

#include "TransitionProfile.h"

namespace omalg {
  
  template<> class TransitionProfile<NondeterministicBuechiAutomaton> {
  private:
    std::vector<std::set<std::pair<size_t, bool> representation;
  public:
    TransitionProfile(std::vector<std::set<std::pair<size_t, bool> > > theRepresentation)
      : representation(theRepresentation) { }
    TransitionProfile concat(TransitionProfile const& rhs) const {
      std::vector<std::set<std::pair<size_t, bool> > > newRepresentation(this->representation.size());
      for (size_t state = 0; state < newRepresentation.size(); ++state) {
        std::set<std::pair<size_t,bool> > intermediates = this->representation[state];
        std::set<std::pair<size_t,bool> >::const_iterator iter;
        for (iter = intermediates.begin(); iter != intermediates.end(); ++iter) {
          std::set<std::pair<size_t,bool> > iterTargets = rhs[iter->first];
          std::set<std::pair<size_t,bool> >::const_iterator targetIter;
          for (targetIter = iterTargets.begin(); targetIter != iterTargets.end(); ++targetIter) {
            size_t target = targetIter->first;
            bool isFinal = iter->second || targetIter->second;
            newRepresentation[state].insert(std::make_pair(target, isFinal));
          }
        }
      }
      return TransitionProfile(newRepresentation);
    }
    
    OmegaProfile mixedProduct(OmegaProfile const& rhs) const {
      
    }
    
    OmegaProfile omegaIteration() const {
      
    }
    std::set<std::pair<size_t,bool> > operator[](size_t index) const {
      return this->representation[index];
    }
  };
  
}
#endif	

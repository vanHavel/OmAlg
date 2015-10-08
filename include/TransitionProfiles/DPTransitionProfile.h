#ifndef OMALG_DPTRANSITIONPROFILE
#define	OMALG_DPTRANSITIONPROFILE

#include "TransitionProfile.h"

namespace omalg {
  
  template<> class TransitionProfile<DeterministicParityAutomaton> {
  private:
    std::vector<std::pair<size_t,size_t> > representation;
  public:
    TransitionProfile(std::vector<std::pair<size_t,size_t> > theRepresentation)
      : representation(theRepresentation) { }
      
    TransitionProfile concat(TransitionProfile const& rhs) const {
      std::vector<std::pair<size_t,size_t> > newRepresentation(this->representation.size());
      for (size_t i = 0; i != newRepresentation.size(); ++i) {
        std::pair<size_t,size_t> intermediate = this->representation[i];
        size_t target = rhs[intermediate.first].first;
        size_t priority = std::max(intermediate.second, rhs[intermediate.first].second);
        newRepresentation[i] = std::make_pair(target, priority);
      }
      return TransitionProfile(newRepresentation);
    }
    
    OmegaProfile mixedProduct(OmegaProfile const& rhs) const {
      std::vector<bool> newRepresentation(this->representation.size());
      for (size_t i = 0; i != newRepresentation.size(); ++i) {
        newRepresentation[i] = rhs[this->representation[i].first];
      }
      return OmegaProfile(newRepresentation);
    }
    
    OmegaProfile omegaIteration() const {
      std::vector<bool> newRepresentation(this->representation.size());
      //For each index: find cycle using variation of tortoise hare algorithm.
      for (size_t i = 0; i != newRepresentation.size(); ++i) {
        size_t tortoise = this->representation[i].first;
        size_t hare = this->representation[tortoise].first;
        while (tortoise != hare) {
          tortoise = this->representation[tortoise].first;
          hare = this->representation[this->representation[hare].first].first;
        }
        //Cycle detected calculate priority on the cycle.
        size_t priority = this->representation[tortoise].second;
        tortoise = this->representation[tortoise].first;
        while (tortoise != hare) {
          priority = std::max(priority, this->representation[tortoise].second);
          tortoise = this->representation[tortoise].first;
        }
        //Check for evenness.
        newRepresentation[i] = (priority % 2 == 0);
      }
      return OmegaProfile(newRepresentation);
    }
    
    std::pair<size_t,size_t> operator[](size_t index) const {
      return this->representation[index];
    }
    
    friend bool operator==(TransitionProfile const& lhs,
                           TransitionProfile const& rhs);
  };
  
  inline bool operator==(TransitionProfile<DeterministicParityAutomaton> const& lhs,
                         TransitionProfile<DeterministicParityAutomaton> const& rhs) {
    return lhs.representation == rhs.representation;
  }
}
#endif	

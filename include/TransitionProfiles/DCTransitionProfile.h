#ifndef OMALG_DCTRANSITIONPROFILE
#define	OMALG_DCTRANSITIONPROFILE

#include "TransitionProfile.h"

namespace omalg {
  
  template<> class TransitionProfile<DeterministicCoBuechiAutomaton> {
  private:
    std::vector<std::pair<size_t,bool> > representation;
  public:
    TransitionProfile(std::vector<std::pair<size_t,bool> > theRepresentation)
      : representation(theRepresentation) { }
      
    TransitionProfile concat(TransitionProfile const& rhs) const {
      std::vector<std::pair<size_t,bool> > newRepresentation(this->representation.size());
      for (size_t i = 0; i != newRepresentation.size(); ++i) {
        std::pair<size_t,bool> intermediate = this->representation[i];
        size_t target = rhs[intermediate.first].first;
        bool isFinal = intermediate.second && rhs[intermediate.first].second;
        newRepresentation[i] = std::make_pair(target, isFinal);
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
      //for each index: find cycle using variation of tortoise hare algorithm.
      for (size_t i = 0; i != newRepresentation.size(); ++i) {
        size_t tortoise = this->representation[i].first;
        size_t hare = this->representation[tortoise].first;
        while (tortoise != hare) {
          tortoise = this->representation[tortoise].first;
          hare = this->representation[this->representation[hare].first].first;
        }
        //Cycle detected, check only final states are visited in the cycle
        bool isFinal = this->representation[tortoise].second;
        tortoise = this->representation[tortoise].first;
        while (isFinal && tortoise != hare) {
          isFinal = isFinal && this->representation[tortoise].second;
          tortoise = this->representation[tortoise].first;
        }
        newRepresentation[i] = isFinal;
      }
      return OmegaProfile(newRepresentation);
    }
    
    std::pair<size_t,bool> operator[](size_t index) const {
      return this->representation[index];
    }
  };
  
}
#endif	

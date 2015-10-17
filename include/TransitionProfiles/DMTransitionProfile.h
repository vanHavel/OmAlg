#ifndef OMALG_DMTRANSITIONPROFILE
#define	OMALG_DMTRANSITIONPROFILE

#include "TransitionProfile.h"

namespace omalg {
  
  template<> class TransitionProfile<DeterministicMullerAutomaton> {
  private:
    std::vector<std::pair<size_t,std::set<size_t> > > representation;
    //Reference to Muller table, needed for omega iteration.
    std::set<std::set<size_t> > const* table;
  public:
    TransitionProfile(std::vector<std::pair<size_t,std::set<size_t> > > theRepresentation,
                      std::set<std::set<size_t> > const* theTable)
      : representation(theRepresentation), table(theTable) { }
      
    TransitionProfile concat(TransitionProfile const& rhs) const {
      std::vector<std::pair<size_t,std::set<size_t> > > newRepresentation(this->representation.size());
      for (size_t i = 0; i != newRepresentation.size(); ++i) {
        std::pair<size_t,std::set<size_t> > intermediate = this->representation[i];
        size_t target = rhs[intermediate.first].first;
        std::set<size_t> visited = rhs[intermediate.first].second;
        visited.insert(intermediate.second.begin(), intermediate.second.end());
        newRepresentation[i] = std::make_pair(target, visited);
      }
      return TransitionProfile(newRepresentation, this->table);
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
        //Cycle detected: calculate state set on the cycle.
        std::set<size_t> visited = this->representation[tortoise].second;
        tortoise = this->representation[tortoise].first;
        while (tortoise != hare) {
          visited.insert(this->representation[tortoise].second.begin(), this->representation[tortoise].second.end());
          tortoise = this->representation[tortoise].first;
        }
        //Check if set is final.
        newRepresentation[i] = (table->find(visited) != table->end());
      }
      return OmegaProfile(newRepresentation);
    }
    
    std::pair<size_t,std::set<size_t> > operator[](size_t index) const {
      return this->representation[index];
    }
    
    friend bool operator==(TransitionProfile const& lhs,
                           TransitionProfile const& rhs);
  };
  
  inline bool operator==(TransitionProfile<DeterministicMullerAutomaton> const& lhs,
                         TransitionProfile<DeterministicMullerAutomaton> const& rhs) {
    return lhs.representation == rhs.representation;
  }
}
#endif	

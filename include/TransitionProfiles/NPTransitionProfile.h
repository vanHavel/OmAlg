#ifndef OMALG_NPTRANSITION_PROFILE
#define	OMALG_NPTRANSITION_PROFILE

#include <vector>
#include <set>
#include <stack>
#include <map>

#include "TransitionProfile.h"

namespace omalg {
  
  template<> class TransitionProfile<NondeterministicParityAutomaton> {
  private:
    std::map<size_t, std::set<std::pair<size_t,size_t> > > representation;
    
  public:
    TransitionProfile(std::map<size_t, std::set<std::pair<size_t,size_t> > > theRepresentation)
      : representation(theRepresentation) { }
      
    TransitionProfile concat(TransitionProfile const& rhs) const {
      std::map<size_t, std::set<std::pair<size_t,size_t> > > newRepresentation;
      //TODO
      return TransitionProfile(newRepresentation);
    }
    
    OmegaProfile mixedProduct(OmegaProfile const& rhs) const {
      std::vector<bool> result(this->representation.size(), false);
      //TODO
      return OmegaProfile(result);
    }
    
    OmegaProfile omegaIteration() const {
      std::vector<bool> result(this->representation.size(), false);
      //TODO
      return OmegaProfile(result);
    }
    
    std::set<std::pair<size_t,size_t> > operator[](size_t index) const {
      if (this->representation.find(index) != this->representation.end()) {
        return this->representation.find(index)->second;
      }
      else {
        //Return empty set.
        return std::set<std::pair<size_t,size_t> >();
      }
    }
    
    friend bool operator==(TransitionProfile const& lhs,
                           TransitionProfile const& rhs);
    
  };
  
  inline bool operator==(TransitionProfile<NondeterministicParityAutomaton> const& lhs,
                         TransitionProfile<NondeterministicParityAutomaton> const& rhs) {
    return lhs.representation == rhs.representation;
  }
}
#endif	

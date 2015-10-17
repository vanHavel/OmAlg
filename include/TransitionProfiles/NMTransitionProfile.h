#ifndef OMALG_NMTRANSITION_PROFILE
#define	OMALG_NMTRANSITION_PROFILE

#include <vector>
#include <set>

#include "TransitionProfile.h"

namespace omalg {
  
  template<> class TransitionProfile<NondeterministicMullerAutomaton> {
  private:
    std::vector<std::set<std::pair<size_t,std::set<size_t> > > > representation;
    std::set<std::set<size_t> > const* table;
    
  public:
    TransitionProfile(std::vector<std::set<std::pair<size_t,std::set<size_t> > > > theRepresentation,
                      std::set<std::set<size_t> > const* theTable)
      : representation(theRepresentation), table(theTable) { }
      
    TransitionProfile concat(TransitionProfile const& rhs) const {
      std::vector<std::set<std::pair<size_t,std::set<size_t> > > > newRepresentation;
      //TODO
      return TransitionProfile(newRepresentation, this->table);
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
    
    std::set<std::pair<size_t,std::set<size_t> > > operator[](size_t index) const {
      return this->representation[index];
    }
    
    friend bool operator==(TransitionProfile const& lhs,
                           TransitionProfile const& rhs);
    
  };
  
  inline bool operator==(TransitionProfile<NondeterministicMullerAutomaton> const& lhs,
                         TransitionProfile<NondeterministicMullerAutomaton> const& rhs) {
    return lhs.representation == rhs.representation;
  }
}
#endif	

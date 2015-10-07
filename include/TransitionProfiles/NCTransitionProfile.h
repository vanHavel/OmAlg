#ifndef OMALG_NCTRANSITION_PROFILE
#define	OMALG_NCTRANSITION_PROFILE

#include <vector>
#include <set>
#include <stack>

#include "TransitionProfile.h"

namespace omalg {
  
  template<> class TransitionProfile<NondeterministicCoBuechiAutomaton> {
  private:
    std::vector<std::set<std::pair<size_t, bool> > > representation;
    
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
            bool isFinal = iter->second && targetIter->second;
            newRepresentation[state].insert(std::make_pair(target, isFinal));
          }
        }
      }
      return TransitionProfile(newRepresentation);
    }
    
    OmegaProfile mixedProduct(OmegaProfile const& rhs) const {
      std::vector<bool> result(this->representation.size(), false);
      for (size_t state = 0; state < this->representation.size(); ++state) {
        std::set<std::pair<size_t, bool> >::const_iterator iter;
        bool done = false;
        for (iter = this->representation[state].begin();
             iter != this->representation[state].end() && !done;
             ++iter) {
          if (rhs[iter->first]) {
            result[state] = true;
            done = true;
          }
        }
      }
      return result;
    }
    
    OmegaProfile omegaIteration() const {
      //At first: identify nodes on a cycle with only "true"-labeled edges.
      //To do this, we compute via depth first search for each state which states
      //can be reached using only true edges.
      std::vector<bool> statesOnAcceptingLoop(this->representation.size(), false);
      for (size_t state = 0; state < this->representation.size(); ++state) {
        std::vector<bool> trueReachableFromState(this->representation.size(), false);
        std::stack<size_t> toCheck;
        std::set<std::pair<size_t,bool> >::const_iterator edgeIter;
        bool done = false;
        //Push nodes initial to check.
        for (edgeIter = this->representation[state].begin();
             edgeIter != this->representation[state].end() && !done;
             ++edgeIter) {
          if (edgeIter->second) {
            toCheck.push(edgeIter->first);
            trueReachableFromState[edgeIter->first] = true;
            if (edgeIter->first == state) {
              done = true;
            }
          }
        }
        while (!toCheck.empty() &&!done) {
          size_t current = toCheck.top();
          toCheck.pop();
          for (edgeIter = this->representation[current].begin();
               edgeIter != this->representation[current].end() && !done;
               ++edgeIter) {
            if (edgeIter->second && !trueReachableFromState[edgeIter->first]) {
              toCheck.push(edgeIter->first);
              trueReachableFromState[edgeIter->first] = true;
              if (edgeIter->first == state) {
                done = true;
            }
            }
          }
        }
        if (trueReachableFromState[state]) {
          statesOnAcceptingLoop[state] = true;
        }
      }
      //Vector for the result
      std::vector<bool> result(this->representation.size(), false);
      //For each state we check whether a state on an accepting loop is reachable
      for (size_t state = 0; state < this->representation.size(); ++state) {
        if (statesOnAcceptingLoop[state]) {
          result[state] = true;
        } 
        else {
          bool done = false;
          std::vector<bool> reachableStates(this->representation.size(), false);
          std::stack<size_t> toCheck;
          toCheck.push(state);
          reachableStates[state] = true;
          while (!toCheck.empty() && !done) {
            size_t current = toCheck.top();
            toCheck.pop();
            if (statesOnAcceptingLoop[current]) {
              //Found an accepting cycle
              result[state] = true;
              done = true;
            }
            else {
              //Continue search
              std::set<std::pair<size_t,bool> >::const_iterator iter;
              for (iter = this->representation[current].begin(); iter != this->representation[current].end(); ++iter) {
                //Add states not yet visited to stack
                if (reachableStates[iter->first] == false) {
                  toCheck.push(iter->first);
                  reachableStates[iter->first] = true;
                }
              }
            }
          } 
        }
      } 
      return OmegaProfile(result);
    }
    
    std::set<std::pair<size_t,bool> > operator[](size_t index) const {
      return this->representation[index];
    }
    
  };
  
}
#endif	

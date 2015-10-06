#ifndef OMALG_NBTRANSITION_PROFILE
#define	OMALG_NBTRANSITION_PROFILE

#include <vector>
#include <set>
#include <stack>

#include "TransitionProfile.h"

namespace omalg {
  
  //Node struct for Tarjan's SCC algorithm
  struct Node {
    Node() : index(-1), lowlink(-1), onStack(false), originalIndex(0) {}
    int index;
    int lowlink;
    bool onStack;
    size_t originalIndex;
  };
  
  template<> class TransitionProfile<NondeterministicBuechiAutomaton> {
  private:
    std::vector<std::set<std::pair<size_t, bool> > > representation;
    
    std::set<std::set<size_t> > getStronglyConnectedComponents() const {
      //Tarjan's SCC algorithm
      std::vector<Node> nodes(this->representation.size());
      size_t index = 0;
      std::stack<Node> nodeStack;
      std::set<std::set<size_t> > components;
      for (size_t state = 0; state < this->representation.size(); ++state) {
        if (nodes[state].index == -1) {
          strongConnect(state, index, nodes, nodeStack, components);
        }
      }
      return components;
    }
    
    void strongConnect(size_t state, size_t index, std::vector<Node>& nodes, 
                       std::stack<Node>& nodeStack, std::set<std::set<size_t> >& components) const {
      nodes[state].index = index;
      nodes[state].lowlink = index;
      nodes[state].originalIndex = state;
      ++index;
      nodeStack.push(nodes[state]);
      nodes[state].onStack = true;
      std::set<std::pair<size_t,bool> >::const_iterator iter;
      for (iter = this->representation[state].begin(); iter != this->representation[state].end(); ++iter) {
        if (nodes[iter->first].index == -1) {
          strongConnect(iter->first, index, nodes, nodeStack, components);
          nodes[state].lowlink = std::min(nodes[state].lowlink, nodes[iter->first].lowlink);
        }
      }
      if (nodes[state].lowlink == nodes[state].index) {
        std::set<size_t> newComponent;
        Node w;
        do {
          w = nodeStack.top();
          nodeStack.pop();
          w.onStack = false;
          newComponent.insert(w.originalIndex);
        }
        while (w.originalIndex != state);
        components.insert(newComponent);
      }
    }
    
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
      //At first: identify nodes on a cycle with a "true"-labeled edge
      std::vector<bool> statesOnAcceptingLoop(this->representation.size(), false);
      //Easy case: self loops labeled "true"
      for (size_t state = 0; state < this->representation.size(); ++state) {
        std::set<std::pair<size_t,bool> >::const_iterator iter;
        for (iter = this->representation[state].begin(); iter != this->representation[state].end(); ++iter) {
          if (iter->first == state && iter->second == true) {
            statesOnAcceptingLoop[state] = true;
          }
        }
      }
      //General Case: detect cycles with a "true" edge. For this we first
      //compute the strongly connected components of the graph
      std::set<std::set<size_t> > stronglyConnectedComponents = this->getStronglyConnectedComponents();
      //We then check for component-internal edges labeled "true"
      std::set<std::set<size_t> >::const_iterator outerIter;
      for (outerIter = stronglyConnectedComponents.begin();
           outerIter != stronglyConnectedComponents.end();
           ++outerIter) {
        std::set<size_t>::const_iterator innerIter;
        bool done = false;
        for (innerIter = outerIter->begin(); innerIter != outerIter->end() && !done; ++innerIter) {
          std::set<std::pair<size_t, bool> >::const_iterator edgeIter;
          for (edgeIter = this->representation[*innerIter].begin(); 
               edgeIter != this->representation[*innerIter].end() && !done;
               ++edgeIter) {
            //if edge is internal and true: mark whole component as accepting
            if (edgeIter->second == true && outerIter->find(edgeIter->first) != outerIter->end()) {
              std::set<size_t>::const_iterator newInnerIter;
              for (newInnerIter = outerIter->begin(); newInnerIter != outerIter->end(); ++newInnerIter) {
                statesOnAcceptingLoop[*newInnerIter] = true;
              }
              done = true;
            }
          }
        }
      }
      //Vector for the result
      std::vector<bool> result(this->representation.size(), false);
      //For each state we check whether a state on an accepting loop is reachable
      //using depth first search
      for (size_t state = 0; state < this->representation.size(); ++state) {
        if (statesOnAcceptingLoop[state]) {
          result[state] = true;
        } 
        else {
          bool done = false;
          std::vector<bool> reachableStates(this->representation.size(), false);
          std::stack<size_t> toCheck;
          toCheck.push(state);
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
              reachableStates[current] = true;
              std::set<std::pair<size_t,bool> >::const_iterator iter;
              for (iter = this->representation[current].begin(); iter != this->representation[current].end(); ++iter) {
                //Add states not yet visited to stack
                if (reachableStates[iter->first] == false) {
                  toCheck.push(iter->first);
                }
              }
            }
          } 
        }
      } 
      return result;
    }
    
    std::set<std::pair<size_t,bool> > operator[](size_t index) const {
      return this->representation[index];
    }
    
  };
  
}
#endif	

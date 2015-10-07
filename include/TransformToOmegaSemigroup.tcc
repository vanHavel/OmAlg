#ifndef OMALG_TRANSFORM_TO_OMEGA_SEMIGROUP
#define OMALG_TRANSFORM_TO_OMEGA_SEMIGROUP

#include <list>
#include "Node.h"

namespace omalg {
  template<class T> OmegaSemigroup* TransformToOmegaSemigroup(T const& Automaton) {
    /**
     * Initialization of data structures.
     **/
    //Transition profile for the empty word.
    TransitionProfile<T> epsilonProfile(Automaton.getEpsilonProfile());
    //"root" Node in the Cayley graph. Aqquired on the heap for consistency.
    Node<TransitionProfile<T> >* epsilonNode = new Node<TransitionProfile<T> >(epsilonProfile, Automaton.alphabetSize());
    //Signifies whether the epsilon profile is part of the final semigroup.
    bool epsilonInSemigroup = false;
    //List storing pointers to all nodes. Used for iteration, keeping track of
    //already processed nodes, and also for memory management.
    std::list<Node<TransitionProfile<T> >*> nodeList;
    nodeList.insert(nodeList.end(), epsilonNode);
    //Iterator to first element that still needs to be processed of the lists in the node.
    typename std::list<Node<TransitionProfile<T> >*>::const_iterator nextToProcess = nodeList.begin();
    
    //Get transition profiles of each letter once for efficiency.
    //TODO check if initializing each with the epsilon profile is a performance killer.
    std::vector<TransitionProfile<T> > letterProfiles(Automaton.alphabetSize(), epsilonProfile);
    for (size_t letter = 0; letter < Automaton.alphabetSize(); ++letter) {
      letterProfiles[letter] = Automaton.getTransitionProfileForLetter(letter);
    }
    
    //Main loop for building the Cayley Graph.
    while (nextToProcess != nodeList.end()) {
      //Generate successor for each letter
      for (size_t letter = 0; letter < Automaton.alphabetSize(); ++letter) {
        //TODO
      }
    }
    
    return (OmegaSemigroup*) epsilonInSemigroup;
  }
}

#endif
#ifndef OMALG_TRANSFORM_TO_OMEGA_SEMIGROUP
#define OMALG_TRANSFORM_TO_OMEGA_SEMIGROUP

#include <list>
#include <unordered_map>
#include "OmegaSemigroup.h"
#include "Morphism.h"
#include "Node.h"
#include "TransitionProfiles/TransitionProfile.h"

namespace omalg {
  template<class T> OmegaSemigroup* TransformToOmegaSemigroup(T const& Automaton) {
    /**
     * Initialization of data structures.
     **/
    //Transition profile for the empty word.
    TransitionProfile<T> epsilonProfile(Automaton.getEpsilonProfile());
    //Index counter for each newly created node.
    size_t nodeIndex = 0;
    //"root" Node in the Cayley graph. Aqquired on the heap for consistency.
    Node<TransitionProfile<T> >* epsilonNode = new Node<TransitionProfile<T> >(epsilonProfile, Automaton.alphabetSize(), 0, nodeIndex);
    ++nodeIndex;
    //Signifies whether the epsilon profile is part of the final semigroup.
    bool epsilonInSemigroup = false;
    //List storing pointers to all nodes. Used for iteration, keeping track of
    //already processed nodes, and also for memory management. 
    std::list<Node<TransitionProfile<T> >*> nodeList;
    nodeList.insert(nodeList.end(), epsilonNode);
    //Iterator to first element that still needs to be processed of the lists in the node.
    typename std::list<Node<TransitionProfile<T> >*>::const_iterator nextToProcess = nodeList.begin();
    //Alphabet for element names
    std::vector<std::string> alphabet = Automaton.getAlphabet();
    
    //Get transition profiles of each letter once for efficiency.
    //TODO check if initializing each with the epsilon profile is a performance killer.
    std::vector<TransitionProfile<T> > letterProfiles(Automaton.alphabetSize(), epsilonProfile);
    for (size_t letter = 0; letter < Automaton.alphabetSize(); ++letter) {
      letterProfiles[letter] = Automaton.getTransitionProfileForLetter(letter);
    }
    
    /**
     * Main loop for building the Cayley Graph.
     **/
    while (nextToProcess != nodeList.end()) {
      TransitionProfile<T> current = (*nextToProcess)->getValue();
      //Generate successor for each letter.
      for (size_t letter = 0; letter < Automaton.alphabetSize(); ++letter) {
        TransitionProfile<T> letterSuccessor = current.concat(letterProfiles[letter]);
        //Check if this successor is new and add link.
        bool found = false;
        for (auto listIter = nodeList.begin(); listIter != nodeList.end() && !found; ++listIter) {
          if ((*listIter)->equalToValue(letterSuccessor)) {
            //"Old" successor. 
            (*nextToProcess)->setSuccessor(letter, *listIter, false);
            found = true;
            //Check if epsilonNode was rediscovered.
            if (listIter == nodeList.begin()) {
              epsilonInSemigroup = true;
            }
          }
        }
        if (!found) {
          //"New" successor, add to list.
          Node<TransitionProfile<T> >* newNode = new Node<TransitionProfile<T> >(letterSuccessor, Automaton.alphabetSize(), *nextToProcess, nodeIndex);
          ++nodeIndex;
          nodeList.insert(nodeList.end(), newNode);
          (*nextToProcess)->setSuccessor(letter, newNode, true);
        }
      }
      //Advance process iterator.
      ++nextToProcess;
    } 
    
    /** 
     * Build product table.
     **/
    size_t tableSize = (epsilonInSemigroup ? nodeList.size() : nodeList.size() - 1);
    std::vector<std::vector<size_t> > productTable(tableSize, std::vector<size_t>(tableSize));
    std::vector<std::string> elementNames(tableSize);
    size_t columnIndex = 0;
    size_t rowBegin = (epsilonInSemigroup ? 1 : 0);
    typename std::list<Node<TransitionProfile<T> >*>::const_iterator listBegin = nodeList.begin();
    if (!epsilonInSemigroup) {
      ++listBegin;
    }
    //Traverse Cayley graph along true edges. 
    //Special Case: "root". Fill first row and column.
    if (epsilonInSemigroup) {
      for (size_t index = 0; index < tableSize; ++index) {
        productTable[0][index] = index;
        productTable[index][0] = index;
        elementNames[0] = "tp(eps)";
      }
    }
    //Set up for traversal.
    std::list<size_t> letterList;
    int nextIndex = -1;
    Node<TransitionProfile<T> >* currentNode = epsilonNode;
    bool done = false;
    
    //Traversal loop. Nodes are processed when added to the list.
    while(!done) {
      nextIndex = currentNode->nextTrueSucessor(nextIndex + 1);
      if (nextIndex == -1) {
        if (letterList.empty()) {
          done = true;
        }
        else {
          nextIndex = letterList.back();
          letterList.pop_back();
          currentNode = currentNode->getParent(); 
        }
      } 
      else {
        currentNode = (*currentNode)[nextIndex].first;
        letterList.insert(letterList.end(), nextIndex);
        nextIndex = -1;
        columnIndex = currentNode->getIndex() - rowBegin;
        //Get element name
        std::string newName = "tp(";
        elementNames[columnIndex] = newName;
        for(auto pathIter = letterList.begin(); pathIter != letterList.end(); ++pathIter) {
          newName += alphabet[*pathIter];
        }
        newName += ")";
        //Update table
        size_t rowIndex = rowBegin;
        for (auto listIter = listBegin; listIter != nodeList.end(); ++listIter) {
          Node<TransitionProfile<T> >* targetNode = *listIter;
          for(auto pathIter = letterList.begin(); pathIter != letterList.end(); ++pathIter) {
            targetNode = (*targetNode)[*pathIter].first;
          }
          productTable[rowIndex][columnIndex] = targetNode->getIndex();
          ++rowIndex;
        }
      }  
    }
    //Table now finished. Create semigroup.
    Semigroup Splus(elementNames, productTable);
    
    //TODO: This is a bit complicated and might not be the most efficient way.
    //Omega iteration of semigroup elements.
    std::vector<OmegaProfile> omegaIters(tableSize, std::vector<bool>(tableSize));
    size_t omegaIndex = 0;
    for (auto listIter = listBegin; listIter != nodeList.end(); ++listIter) {
      omegaIters[omegaIndex] = (*listIter)->getValue().omegaIteration();
      ++omegaIndex;
    }
    //Map of omega profiles to their indices.
    std::unordered_map<OmegaProfile, size_t, OmegaProfileHash> omegaProfiles;
    //List of omega names, will later be turned into vector.
    std::list<std::string> omegaNames;
    //Fill the map with omega iterations.
    omegaIndex = 0;
    for (auto vecIter = omegaIters.begin(); vecIter != omegaIters.end(); ++vecIter) {
      if (omegaProfiles.find(*vecIter) != omegaProfiles.end()) {
        omegaProfiles[*vecIter] = omegaIndex;
        std::string newName = "(" + elementNames[vecIter - omegaIters.begin()] + ")^w";
        omegaNames.insert(omegaNames.end(), newName);
        ++omegaIndex;
      }
    }
    size_t boundary = omegaIndex;
    //Fill the map with all mixed products. Keep partial mixed product table.
    std::vector<std::vector<size_t> > partialMixedTable(tableSize, std::vector<size_t>(omegaProfiles.size()));
    for (auto listIter = listBegin; listIter != nodeList.end(); ++listIter) {
      for (auto vecIter = omegaIters.begin(); vecIter != omegaIters.end(); ++vecIter) {
        OmegaProfile mixedProduct = (*listIter)->getValue().mixedProduct(*vecIter);
        if (omegaProfiles.find(mixedProduct) != omegaProfiles.end()) {
          omegaProfiles[mixedProduct] = omegaIndex;
          std::string newName = elementNames[(*listIter)->getIndex()] + "(" + elementNames[vecIter - omegaIters.begin()] + ")^w";
          omegaNames.insert(omegaNames.end(), newName);
          ++omegaIndex;
        }
        partialMixedTable[(*listIter)->getIndex() - rowBegin][omegaProfiles[*vecIter]] = omegaProfiles[mixedProduct]; 
      }
    }
    //Create real mixed table.
    std::vector<std::vector<size_t> > mixedTable(tableSize, std::vector<size_t>(omegaProfiles.size()));
    //Copy values from old table. 
    for (size_t splusIndex = 0; splusIndex < tableSize; ++splusIndex) {
      for (size_t somegaIndex = 0; somegaIndex < boundary; ++somegaIndex) {
        mixedTable[splusIndex][somegaIndex] = partialMixedTable[splusIndex][somegaIndex];
      }
    }
    //Fill rest of mixed table.
    for (auto mapIter = omegaProfiles.begin(); mapIter != omegaProfiles.end(); ++mapIter) {
      if (mapIter->second >= boundary) {
        for (auto listIter = listBegin; listIter != nodeList.end(); ++listIter) {
          OmegaProfile mixedProduct = (*listIter)->getValue().mixedProduct(mapIter->first);
          mixedTable[(*listIter)->getIndex()][mapIter->second] = omegaProfiles[mixedProduct];
        }
      }
    }
    
    //Fill omega table.
    std::vector<size_t> omegaTable(tableSize);
    for (size_t splusIndex = 0; splusIndex < tableSize; ++splusIndex) {
      omegaTable[splusIndex] = omegaProfiles[omegaIters[splusIndex]];
    }
    //Create set P.
    std::vector<bool> P(omegaProfiles.size(), false);
    size_t initial = Automaton.getInitialState();
    for (auto mapIter = omegaProfiles.begin(); mapIter != omegaProfiles.end(); ++mapIter) {
      if (mapIter->first[initial]) {
        P[mapIter->second] = true;
      }
    }
    
    //Create morphism phi.
    std::vector<size_t> phiValues;
    for (size_t letter = 0; letter < alphabet.size(); ++letter) {
      phiValues[letter] = (*epsilonNode)[letter].first->getIndex();
    }
    Morphism phi(phiValues, alphabet);
    
    //Turn names from list into vector.
    std::vector<std::string> nameVector(omegaNames.begin(), omegaNames.end());
    
    //Create omega semigroup.
    OmegaSemigroup* result = new OmegaSemigroup(Splus, nameVector, mixedTable, omegaTable, P, phi);
    
    //Cleanup
    for (auto listIter = nodeList.begin(); listIter != nodeList.end(); ++listIter) {
      delete *listIter;
    }
    return result;
  }
 
}

#endif
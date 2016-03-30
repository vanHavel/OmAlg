#include <queue>
#include <unordered_map>

#include "AutomatonConverter.h"

namespace omalg {
  AutomatonConverter::AutomatonConverter(OmegaSemigroup const &theS)
    : S(theS) { }

  DeterministicParityAutomaton AutomatonConverter::convertToParity() const {
    //Setup semigroup.
    this->S.sPlus.calculateGreenRelations();
    this->S.sPlus.calculateJDepths();
    //Fetch alphabet size
    auto alphabetSize = this->S.phi.size();
    //Auto increasing state ID for created states.
    size_t stateID = 0;
    //Queue of created states that have yet to be processed for transition specification.
    //States are inserted only once after creation, in order of ID.
    auto toCheck = std::queue<std::list<size_t> >();
    //Map of created states (their string value is the key) to their state id.
    auto stateMap = std::unordered_map<std::string, size_t>();
    //List of transition table for each state, in order of state ID.
    auto transitionList = std::list<std::vector<size_t> >();
    //List of parities for each state, in order of state ID.
    auto parityList = std::list<size_t>();
    //List of state names for each state, in order of state ID.
    auto stateList = std::list<std::string>();

    //Create initial state (empty list).
    stateMap[""] = stateID;
    parityList.push_back(0);
    stateList.push_back("<>");
    ++stateID;
    toCheck.push(std::list<size_t>());

    //main loop
    while (!toCheck.empty()) {
      //get next state to process for transitions
      auto currentState = toCheck.front();
      toCheck.pop();
      auto targetVector = std::vector<size_t>(alphabetSize, 0);
      //Generate successor state for each alphabet letter
      for (size_t letter = 0; letter < alphabetSize; ++letter) {
        auto successor = this->successorState(currentState, this->S.phi[letter]);
        auto successorString = this->stringValue(successor);
        //If state does not already exist: create new state
        if (stateMap.find(successorString) == stateMap.end()) {
          stateMap[successorString] = stateID;
          parityList.push_back(this->calculatePriority(successor));
          stateList.push_back(this->stateName(successor));
          ++stateID;
          toCheck.push(successor);
        }
        //Store transition
        targetVector[letter] = stateMap[successorString];
      }
      //Push transition to list
      transitionList.push_back(targetVector);
    }

    //Create state name vector
    auto stateNames = std::vector<std::string>(stateList.begin(), stateList.end());
    //Create transition table
    auto transitionTable = std::vector<std::vector<size_t> >(transitionList.begin(), transitionList.end());
    //Create priority vector
    auto priorities = std::vector<size_t>(parityList.begin(), parityList.end());

    //return result
    return DeterministicParityAutomaton(stateNames, this->S.phi.getAlphabet(), 0, transitionTable, priorities);

  }

  DeterministicCoBuechiAutomaton AutomatonConverter::convertToCoBuechi() const {
    //TODO
  }

  DeterministicBuechiAutomaton AutomatonConverter::convertToWeakBuechi() const {
    //Get element names which are the state names for the automaton
    auto states = this->S.sPlus.getElementNames();
    //Create new initial state
    std::string initialName = "q_0";
    //Change name of initial state if it already occurs in state names
    while (std::find(states.begin(), states.end(), initialName) != states.end()) {
      initialName += "0";
    }
    //Add initial name to state names
    states.push_back(initialName);
    //set initial id
    size_t initial = states.size() - 1;

    //Get alphabet
    auto alphabet = this->S.phi.getAlphabet();

    //Build transition table
    auto transitionTable = std::vector<std::vector<size_t> >(states.size(), std::vector<size_t>(alphabet.size(), 0));
    for (size_t a = 0; a < alphabet.size(); ++a) {
      size_t image_a = this->S.phi[a];
      //anything but initial state
      for (size_t q = 0; q < states.size() - 1; ++q) {
        transitionTable[q][a] = this->S.sPlus.product(q, image_a);
      }
      //initial state
      transitionTable[initial][a] = image_a;
    }

    //Determine final states
    auto finalStates = std::vector<bool>(states.size(),false);
    auto linkedPairs = this->S.sPlus.linkedPairs();
    for (auto iter = linkedPairs.begin(); iter != linkedPairs.end(); ++iter) {
      size_t s = iter->first;
      size_t eOm = this->S.omegaIterationTable[iter->second];
      //if se^w is in P for a linked pair (s,e), then s is a final state
      if (this->S.P[this->S.mixedProductTable[s][eOm]]) {
        finalStates[s] = true;
      }
    }

    return DeterministicBuechiAutomaton(states, alphabet, initial, transitionTable, finalStates);
  }

  std::string AutomatonConverter::stringValue(std::list<size_t> const& L) const {
    std::string res = "";
    for (auto iter = L.begin(); iter != L.end(); ++iter) {
      res += std::to_string(*iter);
    }
    return res;
  }

  std::list<size_t> AutomatonConverter::successorState(std::list<size_t> currentState, size_t element) const {
    //Special case: list is empty.
    if (currentState.empty()) {
      std::list<size_t> result = {element};
      return result;
    }
    else {
      size_t last = currentState.back();
      size_t prod = this->S.sPlus.product(last, element);
      //If the new element is not in a strictly higher J class than the last (first 2 conditions) OR
      //the last element times the new element falls to a deeper J class than the last one's:
      if (!(this->S.sPlus.j(last, element)) ||
           (this->S.sPlus.j(element,last))  ||
          !(this->S.sPlus.j(last, prod))) {
        //Pop last, recurse with product of last and element as the new rightmost element.
        currentState.pop_back();
        return successorState(currentState, prod);
      }
      else {
        //simply add new element to state.
        currentState.push_back(element);
        return currentState;
      }
    }
  }

  size_t AutomatonConverter::calculatePriority(std::list<size_t> const &state) const {
    //basis priority: 2 times j depth of last element.
    size_t last = state.back();
    size_t depth = this->S.sPlus.jDepth(last);
    size_t res = 2 * depth;
    //Get an idempotent in the R class of last. If there is none, add 1 to priority.
    int e = -1;
    auto idempotents = this->S.sPlus.idempotents();
    for (auto iter = idempotents.begin(); iter != idempotents.end(); ++iter) {
      if (this->S.sPlus.R(last, *iter)) {
        e = *iter;
        break;
      }
    }
    if (e == -1) {
      return res + 1;
    }
    else {
      //Case distinction: states with only one element.
      if (state.size() == 1) {
        //Add 1 if e^w is not in P.
        return res + (this->S.P[this->S.omegaIterationTable[e]] ? 0 : 1);
      }
      else {
        //Calculate the front s.
        size_t s = state.front();
        for (auto iter = ++state.begin(); *iter != last; ++iter) {
          s = this->S.sPlus.product(s, *iter);
        }
        //Calculate se^w.
        size_t seOm = this->S.mixedProductTable[s][this->S.omegaIterationTable[e]];
        //Add 1 if se^w not in P.
        return res + (this->S.P[seOm] ? 0 : 1);
      }
    }

  }

  std::string AutomatonConverter::stateName(std::list<size_t> const &state) const {
    std::string res = "<";
    for (auto iter = state.begin(); iter != state.end(); ++iter) {
      res += this->S.sPlus.elementName(*iter);
      res += ",";
    }
    //remove last ','.
    res.pop_back();
    res += ">";
    return res;
  }


}
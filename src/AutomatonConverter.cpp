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
    //Setup semigroup.
    this->S.sPlus.calculateGreenRelations();
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
    //List of final flag for each state, in order of state ID.
    auto finalList = std::list<bool>();
    //List of state names for each state, in order of state ID.
    auto stateList = std::list<std::string>();

    //Create initial state (empty list).
    stateMap[""] = stateID;
    finalList.push_back(false);
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
        auto successor = this->CBAsuccessorState(currentState, this->S.phi[letter]);
        auto successorString = this->stringValue(successor);
        //If state does not already exist: create new state
        if (stateMap.find(successorString) == stateMap.end()) {
          stateMap[successorString] = stateID;
          finalList.push_back(this->decideFinality(successor));
          stateList.push_back(this->stateName(successor));
          ++stateID;
          toCheck.push(successor);
        }
        //Store transition
        targetVector[letter] = stateMap[successorString];
      }
      //push transition vector to list
      transitionList.push_back(targetVector);
    }

    //Create state name vector
    auto stateNames = std::vector<std::string>(stateList.begin(), stateList.end());
    //Create transition table
    auto transitionTable = std::vector<std::vector<size_t> >(transitionList.begin(), transitionList.end());
    //Create final state vector
    auto finalStates = std::vector<bool>(finalList.begin(), finalList.end());

    //return result
    return DeterministicCoBuechiAutomaton(stateNames, this->S.phi.getAlphabet(), 0, transitionTable, finalStates);

  }

  DeterministicBuechiAutomaton AutomatonConverter::convertToDetBuechi() const {
    this->S.invertP();
    auto compAut = this->convertToCoBuechi();
    this->S.invertP();
    return compAut.dual();
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
          !(this->S.sPlus.r(last, prod))) {
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

  std::list<size_t> AutomatonConverter::CBAsuccessorState(std::list<size_t> const& currentState, size_t element) const {
    if (currentState.size() == 0) {
      //initial state
      std::list<size_t> result = {element};
      return result;
    }
    else if (currentState.size() == 1){
      //single element states
      size_t s = currentState.front();
      size_t se = this->S.sPlus.product(s, element);
      //Check s <_j element, s J s*element and s not sub loop accepting, (s, element) sub loop accepting
      if (this->S.sPlus.j(s, element) && !(this->S.sPlus.j(element, s)) && this->S.sPlus.r(s,se) &&
          !(this->subLoopAccepting(s)) && this->subLoopAccepting(s, element)) {
        std::list<size_t> result = {s, element};
        return result;
      }
      else {
        std::list<size_t> result = {se};
        return result;
      }
    }
    else {
    //double element states
      size_t s = currentState.front();
      size_t t = currentState.back();
      size_t te = this->S.sPlus.product(t, element);
      size_t ste = this->S.sPlus.product(s, te);
      //Check s <_j t*element, s J s*t*element and (s, t*element) sub loop accepting
      if (this->S.sPlus.j(s, te) && !(this->S.sPlus.j(te, s)) && this->S.sPlus.r(s, ste) &&
          this->subLoopAccepting(s, te)) {
        std::list<size_t> result = {s, te};
        return result;
      }
      else {
        std::list<size_t> result = {ste};
        return result;
      }
    }
  }

  bool AutomatonConverter::decideFinality(std::list<size_t> const &state) const {
    //states of Q2 are always final
    if (state.size() == 2) {
      return true;
    }
    else {
      return this->subLoopAccepting(state.front());
    }
  }

  bool AutomatonConverter::subLoopAccepting(size_t t) const {
    auto idem = this->S.sPlus.idempotents();
    //Check idempotents in the same R class for acceptance.
    for (auto iter = idem.begin(); iter != idem.end(); ++iter) {
      if (this->S.sPlus.R(t, *iter)) {
        return this->S.P[this->S.omegaIterationTable[*iter]];
      }
    }
    //R class is irregular -> check above linked pairs.
    auto pairs = this->S.sPlus.linkedPairs();
    for (auto iter = pairs.begin(); iter != pairs.end(); ++iter) {
      size_t r = iter->first;
      size_t e = iter->second;
      size_t tr = this->S.sPlus.product(t, r);
      //Check if t <_j r and tr J t.
      if (this->S.sPlus.j(t, r) && !(this->S.sPlus.j(r, t)) && this->S.sPlus.r(t, tr)) {
        //Check if tre^w in P for sub loop acceptance.
        size_t eOm = this->S.omegaIterationTable[e];
        if(!(this->S.P[this->S.mixedProductTable[tr][eOm]])) {
          return false;
        }
      }
    }
    //All above linked pairs are accepting.
    return true;
  }

  bool AutomatonConverter::subLoopAccepting(size_t s, size_t t) const {
    auto idem = this->S.sPlus.idempotents();
    //Check idempotents in the same R class for acceptance.
    for (auto iter = idem.begin(); iter != idem.end(); ++iter) {
      if (this->S.sPlus.R(t, *iter)) {
        size_t eOm = this->S.omegaIterationTable[*iter];
        return this->S.P[this->S.mixedProductTable[s][eOm]];
      }
    }
    //R class is irregular -> check above linked pairs.
    auto pairs = this->S.sPlus.linkedPairs();
    for (auto iter = pairs.begin(); iter != pairs.end(); ++iter) {
      size_t r = iter->first;
      size_t e = iter->second;
      size_t tr = this->S.sPlus.product(t, r);
      size_t str = this->S.sPlus.product(s, tr);
      //Check if t <_j r and tr J t.
      if (this->S.sPlus.j(t, r) && !(this->S.sPlus.j(r, t)) && this->S.sPlus.r(t, tr)) {
        //Check if stre^w in P for sub loop acceptance.
        size_t eOm = this->S.omegaIterationTable[e];
        if(!(this->S.P[this->S.mixedProductTable[str][eOm]])) {
          return false;
        }
      }
    }
    //All above linked pairs are accepting.
    return true;
  }

}
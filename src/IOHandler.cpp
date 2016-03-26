#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>

#include "IOHandler.h"
#include "IOHandlerExceptions.h"
#include "vanHavel_Util.h"
#include "DeterministicBuechiAutomaton.h"
#include "NondeterministicBuechiAutomaton.h"
#include "DeterministicCoBuechiAutomaton.h"
#include "NondeterministicCoBuechiAutomaton.h"
#include "DeterministicParityAutomaton.h"
#include "NondeterministicParityAutomaton.h"
#include "DeterministicMullerAutomaton.h"
#include "NondeterministicMullerAutomaton.h"

namespace omalg {

  IOHandler::IOHandler() {
    std::vector<std::string> acceptanceModes;
    acceptanceModes.push_back("buechi");
    acceptanceModes.push_back("cobuechi");
    acceptanceModes.push_back("parity");
    acceptanceModes.push_back("muller");
    this->possibleModes = acceptanceModes;
  }
  
  OmegaAutomaton* IOHandler::readAutomatonFromStream(std::istream &in) {
    std::vector<std::string> lines;
    std::string temp;
    const size_t minLines = 7;

    //Read complete file
    while (std::getline(in, temp)) {
      lines.push_back(temp);
    }
    //Check whether fail occurred for other reason but eof
    if (!in.eof()) {
      throw ReadFailedException();
    }
    //Clear fail and bad bits
    in.clear();
    //Check minimum number of lines
    if (lines.size() < minLines) {
      throw SyntaxException(lines.size(), "File must have at least " + std::to_string(minLines) + " lines.");
    }

    //Read automaton acceptance mode.
    size_t lineNo = 0;
    std::string acceptanceMode = lines[lineNo];
    //Check that last symbol is ';' and erase it.
    if(acceptanceMode.back() == ';') {
      acceptanceMode.pop_back();
    }
    else {
      throw SyntaxException(lineNo, "Expected ';'.");
    }
    //Transform to lower case.
    std::transform(acceptanceMode.begin(), acceptanceMode.end(), acceptanceMode.begin(), ::tolower);
    //Check if mode is supported, throw syntax exception otherwise.
    if (std::find(this->possibleModes.begin(), this->possibleModes.end(), acceptanceMode) == this->possibleModes.end()) {
      std::string errorText = "Expected one of the following acceptance modes:";
      std::vector<std::string>::const_iterator it;
      for(it = this->possibleModes.begin(); it != this->possibleModes.end(); ++it) {
        errorText += " ";
        errorText += *it;
      }
      errorText += ".";
      throw SyntaxException(lineNo + 1, errorText);
    }

    //Read whether automaton is deterministic.
    bool deterministic = false;
    ++lineNo;
    temp = lines[lineNo];
    //Check that last symbol is ';' and erase it.
    if(temp.back() == ';') {
      temp.pop_back();
    }
    else {
      throw SyntaxException(lineNo + 1, "Expected ';'.");
    }
    std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
    if (temp.compare("deterministic") == 0) {
      deterministic = true;
    }
    else if (temp.compare("nondeterministic") == 0) {
      deterministic = false;
    }
    //Throw syntax error on erroneous input.
    else {
      throw SyntaxException(lineNo, "Expected 'deterministic' or 'nondeterministic'");
    }

    //Read list of states
    lineNo++;
    std::list<std::string> stateNames = this->readNamesIntoList(lines, lineNo);
    //Read initial state
    ++lineNo;
    this->checkReadTillEnd(lineNo, lines.size());
    //Check that last symbol is ';' and erase it.
    std::string initialName = lines[lineNo];
    if(initialName.back() == ';') {
      initialName.pop_back();
    }
    else {
      throw SyntaxException(lineNo + 1, "Expected ';'.");
    }
    //Check that initial state is in state list
    if (std::find(stateNames.begin(), stateNames.end(), initialName) == stateNames.end()) {
      throw SyntaxException(lineNo + 1, "Initial state " + initialName + " not in state set.");
    }

    //Read alphabet
    ++lineNo;
    std::list<std::string> letterNames = this->readNamesIntoList(lines, lineNo);

    //Read transitions
    ++lineNo;
    int transNo = lineNo;
    std::list<std::string> transitionTriplets = this->readNamesIntoList(lines, lineNo, 1);

    //Build state/letter vectors and get initial number
    std::vector<std::string> stateVector(stateNames.begin(), stateNames.end());
    std::vector<std::string> letterVector(letterNames.begin(), letterNames.end());
    int initialState = dasdull::vectorPos(stateVector, initialName);

    //Read acceptance component and build automaton
    if (acceptanceMode == "buechi") {
      ++lineNo;
      this->checkReadTillEnd(lineNo, lines.size());
      //Read final state names
      auto finalNames = this->readNamesIntoList(lines, lineNo);
      //Construct final state bit vector
      std::vector<bool> finalStates(stateNames.size(), false);
      for (auto iter = finalNames.begin(); iter != finalNames.end(); ++iter) {
        int pos = dasdull::vectorPos(stateVector, *iter);
        if (pos != -1) {
          finalStates[pos] = true;
        }
        else {
          //Final state not in state set
          throw SyntaxException(lineNo + 1, "Final state " + *iter + " not in state set.");
        }
      }
      //Build automaton
      if (deterministic) {
        auto transitionTable = this->buildTransitionTable(transitionTriplets, stateVector, letterVector, transNo);
        return new DeterministicBuechiAutomaton(stateVector,
                                               letterVector,
                                               initialState,
                                               transitionTable,
                                               finalStates);
      }
      else {
        auto transitionRelation = this->buildTransitionRelation(transitionTriplets, stateVector, letterVector, transNo);
        return new NondeterministicBuechiAutomaton(stateVector,
                                                   letterVector,
                                                   initialState,
                                                   transitionRelation,
                                                   finalStates);
      }
    }
    else if (acceptanceMode == "cobuechi") {
      ++lineNo;
      this->checkReadTillEnd(lineNo, lines.size());
      //Read final state names
      auto finalNames = this->readNamesIntoList(lines, lineNo);
      //Construct final state bit vector
      std::vector<bool> finalStates(stateNames.size(), false);
      for (auto iter = finalNames.begin(); iter != finalNames.end(); ++iter) {
        int pos = dasdull::vectorPos(stateVector, *iter);
        if (pos != -1) {
          finalStates[pos] = true;
        }
        else {
          //Final state not in state set
          throw SyntaxException(lineNo + 1, "Final state " + *iter + " not in state set.");
        }
      }
      //Build automaton
      if (deterministic) {
        auto transitionTable = this->buildTransitionTable(transitionTriplets, stateVector, letterVector, transNo);
        return new DeterministicCoBuechiAutomaton(stateVector,
                                                 letterVector,
                                                 initialState,
                                                 transitionTable,
                                                 finalStates);
      }
      else {
        auto transitionRelation = this->buildTransitionRelation(transitionTriplets, stateVector, letterVector, transNo);
        return new NondeterministicCoBuechiAutomaton(stateVector,
                                                     letterVector,
                                                     initialState,
                                                     transitionRelation,
                                                     finalStates);
      }
    }
    else if (acceptanceMode == "parity") {
      ++lineNo;
      this->checkReadTillEnd(lineNo, lines.size());
      //Read priorities.
      auto prioritiesAsStrings = this->readNamesIntoList(lines, lineNo);
      if (prioritiesAsStrings.size() != stateNames.size()) {
        throw SyntaxException(lineNo + 1, "Number of priorities given does not equal number of states.");
      }
      //Construct priority vector
      std::vector<size_t> priorities(stateNames.size(), 0);
      size_t priority;
      auto iter = prioritiesAsStrings.begin();
      for (size_t index = 0; index < prioritiesAsStrings.size(); ++index) {
        try { 
          priority = std::stoul(*iter);
          ++iter;
        }
        catch (std::exception const& e) {
          throw SyntaxException(lineNo + 1, "Priority " + (*iter) + " is not a non-negative integer.");
        }
        priorities[index] = priority;
      }
      //Build automaton
      if (deterministic) {
        auto transitionTable = this->buildTransitionTable(transitionTriplets, stateVector, letterVector, transNo);
        return new DeterministicParityAutomaton(stateVector,
                                                 letterVector,
                                                 initialState,
                                                 transitionTable,
                                                 priorities);
      }
      else {
        auto transitionRelation = this->buildTransitionRelation(transitionTriplets, stateVector, letterVector, transNo);
        return new NondeterministicParityAutomaton(stateVector,
                                                     letterVector,
                                                     initialState,
                                                     transitionRelation,
                                                     priorities);
      }
    }
    else if (acceptanceMode == "muller") {
      ++lineNo;
      this->checkReadTillEnd(lineNo, lines.size());
      //Read accepting sets
      auto setsAsStrings = this->readNamesIntoList(lines, lineNo, 2);
      //Build Muller automaton table
      std::set<std::set<size_t> > table;
      for (auto outerIter = setsAsStrings.begin(); outerIter != setsAsStrings.end(); ++outerIter) {
        auto newSetAsString = dasdull::stringSplit(*outerIter, ',', true);
        //check for opening and closing curly braces
        if (newSetAsString.front().front() == '{') {
          newSetAsString.front().erase(0,1);
          if (newSetAsString.back().back() == '}') {
            newSetAsString.back().pop_back();
            //build new set for table
            std::set<size_t> newSet;
            for (auto innerIter = newSetAsString.begin(); innerIter != newSetAsString.end(); ++innerIter) {
              int statePos = dasdull::vectorPos(stateVector, *innerIter);
              if (statePos != -1) {
                newSet.insert(newSet.end(), statePos);
              }
              else {
                throw SyntaxException(lineNo + 1, "State " + *innerIter + " of set {" + *outerIter + "} not in state set." + "\n"
                    + "(Might be in a prior line.)");
              }
            }
            table.insert(table.end(), newSet);
          }
          else {
            throw SyntaxException(lineNo + 1, "Expected '}' in state set " + *outerIter + "\n" + "(Might be in a prior line.)");
          }
        }
        else {
          throw SyntaxException(lineNo + 1, "Expected '{' in state set " + *outerIter + "\n" + "(Might be in a prior line.)");
        }
      }
      if (deterministic) {
        auto transitionTable = this->buildTransitionTable(transitionTriplets, stateVector, letterVector, transNo);
        return new DeterministicMullerAutomaton(stateVector,
            letterVector,
            initialState,
            transitionTable,
            table);
      }
      else {
        auto transitionRelation = this->buildTransitionRelation(transitionTriplets, stateVector, letterVector, transNo);
        return new NondeterministicMullerAutomaton(stateVector,
            letterVector,
            initialState,
            transitionRelation,
            table);
      }
    }
    else {
      //This should never be reached in normal execution as automaton types are checked
      throw SyntaxException(lineNo, "Unexpected Automaton type.");
    }
  }
  
  OmegaAutomaton* IOHandler::readAutomatonFromFile(std::string const inputFileName) {
    std::ifstream in;
    in.open(inputFileName, std::ios::in);
    if (!in.good()) {
      throw OpenFailedException(inputFileName);
    }
    //Read automaton from file.
    OmegaAutomaton* result;
    try {
      result = this->readAutomatonFromStream(in);
    }
    catch(IOException const &ex) {
      //An exception was thrown while reading from file.
      //Attempt to close the file.
      in.close();
      throw;
    }
    //Close the input file.
    in.close();
    if (in.fail()) {
      throw CloseFailedException(inputFileName);
    }
    return result;
  }

  OmegaAutomaton* IOHandler::readAutomatonFromStdin() {
    OmegaAutomaton* A;
    A = this->readAutomatonFromStream(std::cin);
    return A;
  }

  OmegaSemigroup* IOHandler::readOmegaSemigroupFromStream(std::istream &in) {
    std::vector<std::string> lines;
    std::string temp;
    const size_t minLines = 8;

    //Read complete file
    while (std::getline(in, temp)) {
      lines.push_back(temp);
    }
    //Check whether fail occurred for other reason but eof
    if (!in.eof()) {
      throw ReadFailedException();
    }
    //Clear fail and bad bits
    in.clear();
    //Check minimum number of lines
    if (lines.size() < minLines) {
      throw SyntaxException(lines.size(), "File must have at least " + std::to_string(minLines) + " lines.");
    }

    size_t lineNo = 0;

    //read semigroup element names
    auto sPlusNamesList = this->readNamesIntoList(lines, lineNo);
    auto sPlusNames = std::vector<std::string>(sPlusNamesList.begin(), sPlusNamesList.end());
    ++lineNo;
    this->checkReadTillEnd(lineNo, lines.size());

    //read semigroup product table
    auto sPlusTable = std::vector<std::vector<size_t> >(sPlusNames.size(), std::vector<size_t>(sPlusNames.size(), 0));
    for (size_t row = 0; row < sPlusNames.size(); ++row) {
      //split row at commas
      auto tableRow = dasdull::stringSplit(lines[lineNo], ',', true);
      size_t col = 0;
      //iterate over row
      for (auto iter = tableRow.begin(); iter != tableRow.end(); ++iter) {
        //check if row contains too many elements
        if (col == sPlusNames.size()) {
          throw SyntaxException(lineNo + 1, "Too many elements in row of product table");
        }
        //get index of element in the semigroup
        int pos = dasdull::vectorPos(sPlusNames, *iter);
        //check whether element occurs at all in semigroup
        if (pos == -1) {
          throw SyntaxException(lineNo + 1, "Element " + *iter + " of product table not in semigroup.");
        }
        //fill table and increase column counter
        else {
          sPlusTable[row][col] = pos;
          ++col;
        }
      }
      //check if there were too few elements in the row
      if (col < sPlusNames.size()) {
        throw SyntaxException(lineNo + 1, "Too few elements in row of product table.");
      }
      //increase line count
      ++lineNo;
      this->checkReadTillEnd(lineNo, lines.size());
    }

    //create semigroup
    Semigroup sPlus = Semigroup(sPlusNames, sPlusTable);

    //read omega element names
    auto omegaNameList = this->readNamesIntoList(lines, lineNo);
    auto omegaNames = std::vector<std::string>(omegaNameList.begin(), omegaNameList.end());
    ++lineNo;
    this->checkReadTillEnd(lineNo, lines.size());

    //read mixed product table
    auto mixedTable = std::vector<std::vector<size_t> >(sPlusNames.size(), std::vector<size_t>(omegaNames.size(), 0));
    for (size_t row = 0; row < sPlusNames.size(); ++row) {
      //split row at commas
      auto tableRow = dasdull::stringSplit(lines[lineNo], ',', true);
      size_t col = 0;
      //iterate over row
      for (auto iter = tableRow.begin(); iter != tableRow.end(); ++iter) {
        //check if row contains too many elements
        if (col == omegaNames.size()) {
          throw SyntaxException(lineNo + 1, "Too many elements in row of mixed product table");
        }
        //get index of omega element in the omega semigroup
        int pos = dasdull::vectorPos(omegaNames, *iter);
        //check whether element occurs at all in omega semigroup
        if (pos == -1) {
          throw SyntaxException(lineNo + 1, "Element " + *iter + " of mixed product table not in omega semigroup.");
        }
        //fill table and increase column counter
        else {
          mixedTable[row][col] = pos;
          ++col;
        }
      }
      //check if there were too few elements in the row
      if (col < omegaNames.size()) {
        throw SyntaxException(lineNo + 1, "Too few elements in row of mixed product table.");
      }
      //increase line count
      ++lineNo;
      this->checkReadTillEnd(lineNo, lines.size());
    }

    //read omega iteration table
    auto omegaTableList = this->readNamesIntoList(lines, lineNo);
    //check correct size of omega table
    if (omegaTableList.size() != sPlusNames.size()) {
      throw SyntaxException(lineNo + 1, "Wrong number of elements in omega iteration table - does not match number of semigroup elements");
    }
    //transform to indices
    auto omegaTable = std::vector<size_t>(omegaTableList.size(), 0);
    size_t index = 0;
    for (auto iter = omegaTableList.begin(); iter != omegaTableList.end(); ++iter) {
      int pos = dasdull::vectorPos(omegaNames, *iter);
      if (pos == -1) {
        throw SyntaxException(lineNo + 1, "Omega iteration result " + *iter + " does not appear in omega semigroup.");
      }
      else {
        //fill table and increase index
        omegaTable[index] = pos;
        ++index;
      }
    }
    ++lineNo;
    this->checkReadTillEnd(lineNo, lines.size());

    //read alphabet
    auto alphabetList = this->readNamesIntoList(lines, lineNo);
    auto alphabet = std::vector<std::string>(alphabetList.begin(), alphabetList.end());
    ++lineNo;
    this->checkReadTillEnd(lineNo, lines.size());

    //read morphism images
    auto morphismList = this->readNamesIntoList(lines, lineNo);
    //check correct size of image table
    if (morphismList.size() != alphabet.size()) {
      throw SyntaxException(lineNo + 1, "Wrong number of elements in morphism table - does not match number of alphabet elements");
    }
    //transform to indices
    auto morphismValues = std::vector<size_t>(morphismList.size(),0);
    index = 0;
    for (auto iter = morphismList.begin(); iter != morphismList.end(); ++iter) {
      int pos = dasdull::vectorPos(sPlusNames, *iter);
      //check if image occurs in sPlus
      if (pos == -1) {
        throw SyntaxException(lineNo + 1, "Morphism target " + *iter + " does not appear in semigroup.");
      }
      else {
        morphismValues[index] = pos;
        ++index;
      }
    }

    //create morphism
    Morphism phi = Morphism(morphismValues, alphabet);
    ++lineNo;
    this->checkReadTillEnd(lineNo, lines.size());

    //read P
    auto PList = this->readNamesIntoList(lines, lineNo);
    auto P = std::vector<bool>(omegaNames.size(), false);
    for (auto iter = PList.begin(); iter != PList.end(); ++iter) {
      int pos = dasdull::vectorPos(omegaNames, *iter);
      //check if P member occurs in omegaNames
      if (pos == -1) {
        throw SyntaxException(lineNo + 1, "P element " + *iter + " does not appear in omega semigroup.");
      }
      else {
        P[pos] = true;
      }
    }

    //create omega semigroup
    OmegaSemigroup* S = new OmegaSemigroup(sPlus, omegaNames, mixedTable, omegaTable, P, phi);
    return S;
  }

  OmegaSemigroup* IOHandler::readOmegaSemigroupFromFile(std::string inputFileName) {
    std::ifstream in;
    in.open(inputFileName, std::ios::in);
    if (!in.good()) {
      throw OpenFailedException(inputFileName);
    }
    //Read omega semigroup from file.
    OmegaSemigroup* result;
    try {
      result = this->readOmegaSemigroupFromStream(in);
    }
    catch(IOException const &ex) {
      //An exception was thrown while reading from file.
      //Attempt to close the file.
      in.close();
      throw;
    }
    //Close the input file.
    in.close();
    if (in.fail()) {
      throw CloseFailedException(inputFileName);
    }
    return result;
  }

  OmegaSemigroup* IOHandler::readOmegaSemigroupFromStdin() {
    OmegaSemigroup* S;
    S = this->readOmegaSemigroupFromStream(std::cin);
    return S;
  }

  void IOHandler::writeAutomatonToStream(OmegaAutomaton const &A, std::ostream& out) {
    out << A.description();  
    if (out.fail()) {
      throw WriteFailedException();
    }
  }

  void IOHandler::writeAutomatonToStdout(OmegaAutomaton const &A) {
    this->writeAutomatonToStream(A, std::cout);
    if(std::cout.fail()) {
      throw WriteFailedException();
    }
  }

  void IOHandler::writeAutomatonToFile(OmegaAutomaton const &A, std::string outputFileName) {
    std::ofstream out;
    out.open(outputFileName, std::ios::out);
    if (!out.good()) {
      throw OpenFailedException(outputFileName);
    }
    //Write automaton to file.
    try {
      this->writeAutomatonToStream(A, out);
    }
    catch(IOException const &ex) {
      //An exception was thrown while writing to file.
      //Attempt to close the file.
      out.close();
      throw;
    }
    //Close the input file.
    out.close();
    if (out.fail()) {
      throw CloseFailedException(outputFileName);
    }
  }

  void IOHandler::writeOmegaSemigroupToStream(OmegaSemigroup const &S, std::ostream& out) {
    out << S.description();  
    if (out.fail()) {
      throw WriteFailedException();
    }
  }

  void IOHandler::writeOmegaSemigroupToFile(OmegaSemigroup const &S, std::string const outputFileName) {
    std::ofstream out;
    out.open(outputFileName, std::ios::out);
    if (!out.good()) {
      throw OpenFailedException(outputFileName);
    }
    //Write automaton to file.
    try {
      this->writeOmegaSemigroupToStream(S, out);
    }
    catch(IOException const &ex) {
      //An exception was thrown while writing to file.
      //Attempt to close the file.
      out.close();
      throw;
    }
    //Close the input file.
    out.close();
    if (out.fail()) {
      throw CloseFailedException(outputFileName);
    }
  }

  void IOHandler::writeOmegaSemigroupToStdout(OmegaSemigroup const &S) {
    this->writeOmegaSemigroupToStream(S, std::cout);
    if(std::cout.fail()) {
      throw WriteFailedException();
    }
  }

  void IOHandler::checkReadTillEnd(size_t lineNo, size_t lines) {
    if (lineNo >= lines) {
      //We Read till end of file
      throw SyntaxException(lineNo, "Reached end of file while parsing.");
    }
  }

  std::list<std::string> IOHandler::readNamesIntoList(std::vector<std::string> const &lines, size_t &lineNo, size_t mode) {
    std::list<std::string> result;
    std::list<std::string> newNames;
    while(lineNo < lines.size() && lines[lineNo].back() != ';') {
      newNames = dasdull::stringSplit(lines[lineNo], ',', true);
      result.insert(result.end(), newNames.begin(), newNames.end());
      ++lineNo;
    }
    this->checkReadTillEnd(lineNo, lines.size());
    //remove ';' from end
    std::string temp = lines[lineNo];
    temp.pop_back();
    newNames = dasdull::stringSplit(temp, ',', true);
    result.insert(result.end(), newNames.begin(), newNames.end());
    //Default: return all strings separated by ';'
    if (mode == 0) {
      return result;
    }
    //Special case: put together transitions
    else if (mode == 1) {
      if (result.size() % 3 != 0) {
        throw SyntaxException(lineNo + 1, "Invalid transition format(Might be at a prior line).");
      }
      std::list<std::string> compressedResult;
      for (auto iter = result.begin(); iter != result.end(); ++iter) {
        std::string transition = *iter;
        iter++;
        transition += "," + *iter;
        iter++;
        transition += "," + *iter;
        compressedResult.insert(compressedResult.end(), transition);
      }
      return compressedResult;
    }
    //Special case: put together state sets
    else if (mode == 2) {
      std::list<std::string> compressedResult;
      for (auto iter = result.begin(); iter != result.end(); ++iter) {
        std::string setString = "";
        while (iter->back() != '}' && iter != result.end()) {
          setString += *iter + ",";
          ++iter;
        }
        if (iter != result.end()) {
          setString += *iter;
        }
        compressedResult.insert(compressedResult.end(), setString);
      }
      return compressedResult;
    }
    else {
      throw std::invalid_argument("Mode must be zero, one or two.");
    }
  }

  std::vector<std::vector<std::set<size_t> > > IOHandler::buildTransitionRelation(std::list<std::string> const &transitions,
                                                                 std::vector<std::string> const &stateVector,
                                                                 std::vector<std::string> const &letterVector,
                                                                 size_t transNo) {
    std::vector<std::vector<std::set<size_t> > > result(stateVector.size(), std::vector<std::set<size_t> >(letterVector.size()));
    for (auto iter = transitions.begin(); iter != transitions.end(); ++iter) {
      std::string transition = *iter;
      //Remove brackets
      if (transition.front() == '(') {
        transition.erase(0,1);
        if (transition.back() == ')') {
          transition.pop_back();
          //Get components
          std::list<std::string> components = dasdull::stringSplit(transition, ',', true);
          if (components.size() == 3) {
            std::string origin = components.front();
            components.pop_front();
            std::string letter = components.front();
            std::string target = components.back();
            int originPos = dasdull::vectorPos(stateVector, origin);
            int letterPos = dasdull::vectorPos(letterVector, letter);
            int targetPos = dasdull::vectorPos(stateVector, target);
            //Check components
            if (originPos == -1) {
              throw SyntaxException(transNo + 1, "Origin state of transition (" + transition + ") not in state set." + "\n" + "(Might be in a later line.)");
            }
            else if (letterPos == -1) {
              throw SyntaxException(transNo + 1, "Letter of transition (" + transition + ") not in alphabet." + "\n" + "(Might be in a later line.)");
            }
            else if (targetPos == -1) {
              throw SyntaxException(transNo + 1, "Target state of transition (" + transition + ") not in state set." + "\n" + "(Might be in a later line.)");
            }
            else {
              //Add to transition relation
              result[originPos][letterPos].insert(targetPos);
            }
          }
          else {
            throw SyntaxException(transNo + 1, "Transition ("+ transition + ") does not have three components." + "\n" + "(Might be in a later line.)");
          }
        }
        else {
          throw SyntaxException(transNo + 1, "Expected ')' in transition (" + transition + "\n" + "(Might be in a later line.)");
        }
      }
      else {
        throw SyntaxException(transNo + 1, "Expected '(' in transition " + transition + "\n" + "(Might be in a later line.)");
      }
    }
    return result;
  }

  std::vector<std::vector<size_t> > IOHandler::buildTransitionTable(std::list<std::string> const &transitions,
                                                                   std::vector<std::string> const &stateVector,
                                                                   std::vector<std::string> const &letterVector,
                                                                   size_t transNo) {
    //initialize with 0
    std::vector<std::vector<size_t> > result(stateVector.size(), std::vector<size_t>(letterVector.size()));
    //vector of defined transitions
    std::vector<std::vector<bool> > isDefined(stateVector.size(), std::vector<bool>(letterVector.size(), false));
    for (auto iter = transitions.begin(); iter != transitions.end(); ++iter) {
      std::string transition = *iter;
      //Remove brackets
      if (transition.front() == '(') {
        transition.erase(0,1);
        if (transition.back() == ')') {
          transition.pop_back();
          //Get components
          std::list<std::string> components = dasdull::stringSplit(transition, ',', true);
          if (components.size() == 3) {
            std::string origin = components.front();
            components.pop_front();
            std::string letter = components.front();
            std::string target = components.back();
            int originPos = dasdull::vectorPos(stateVector, origin);
            int letterPos = dasdull::vectorPos(letterVector, letter);
            int targetPos = dasdull::vectorPos(stateVector, target);
            //Check components
            if (originPos == -1) {
              throw SyntaxException(transNo, "Origin state of transition (" + transition + ") not in state set." + "\n" + "(Might be in a later line.)");
            }
            else if (letterPos == -1) {
              throw SyntaxException(transNo, "Letter of transition (" + transition + ") not in alphabet." + "\n" + "(Might be in a later line.)");
            }
            else if (targetPos == -1) {
              throw SyntaxException(transNo, "Target state of transition (" + transition + ") not in state set." + "\n" + "(Might be in a later line.)");
            }
            else {
              //Add to transition relation
              if (!isDefined[originPos][letterPos]) {
                result[originPos][letterPos] = targetPos;
                isDefined[originPos][letterPos] = true;
              }
              else {
                throw SyntaxException(transNo, "Several transitions for state " + origin + " and letter " + letter + ".\n" + "(Might be in a later line.)");
              }
            }
          }
          else {
            throw SyntaxException(transNo, "Transition ("+ transition + ") does not have three components." + "\n" + "(Might be in a later line.)");
          }
        }
        else {
          throw SyntaxException(transNo, "Expected ')' in transition (" + transition + "\n" + "(Might be in a later line.)");
        }
      }
      else {
        throw SyntaxException(transNo, "Expected '(' in transition " + transition + "\n" + "(Might be in a later line.)");
      }
    }
    //Check for undefined transitions
    for (auto outerIter = isDefined.begin(); outerIter != isDefined.end(); ++outerIter) {
      for (auto innerIter = outerIter->begin(); innerIter != outerIter->end(); ++innerIter) {
        if (*innerIter == false) {
          throw SyntaxException(transNo, "There were undefined transitions.\n(Might be in a later line.)");
        }
      }
    }
    return result;
  }

}

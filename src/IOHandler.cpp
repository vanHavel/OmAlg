#include <iostream>
#include <fstream>
#include <algorithm>

#include "IOHandler.h"
#include "IOHandlerExceptions.h"
#include "dasDull_Util.h"
#include "DeterministicBuechiAutomaton.h"
#include "NondeterministicBuechiAutomaton.h"

namespace omalg {

  IOHandler::IOHandler() {
    std::vector<std::string> acceptanceModes;
    acceptanceModes.push_back("buechi");
    this->possibleModes = acceptanceModes;
  }
  
  OmegaAutomaton* IOHandler::readAutomatonFromStream(std::istream &in) {
    std::vector<std::string> lines;
    std::string temp;
    const int minLines = 7;

    //Read complete file
    while (this->safeGetLine(in, temp)) {
      lines.push_back(temp);
    }
    //Check minimum number of lines
    if (lines.size() < minLines) {
      throw SyntaxException(lines.size(), "File must have at least " + std::to_string(minLines) + " lines.");
    }

    //Read automaton acceptance mode.
    int lineNo = 0;
    std::string acceptanceMode = lines[lineNo];
    //Check that last symbol is ';' and erase it.
    if(acceptanceMode.back() == ';') {
      acceptanceMode.pop_back();
    }
    else {
      throw SyntaxException(lineNo, "Expected ';'.");
    }
    //Transform to lower case.
    std::transform(acceptanceMode.begin(), acceptanceMode.end(), acceptanceMode.end(), ::tolower);
    //Check if mode is supported, throw syntax exception otherwise.
    if (find(this->possibleModes.begin(), this->possibleModes.end(), acceptanceMode) == this->possibleModes.end()) {
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
    std::transform(temp.begin(), temp.end(), temp.end(), ::tolower);
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
    if(temp.back() == ';') {
      temp.pop_back();
    }
    else {
      throw SyntaxException(lineNo + 1, "Expected ';'.");
    }
    std::string initialName = lines[lineNo];
    //Check that initial state is in state list
    if (std::find(stateNames.begin(), stateNames.end(), initialName) == stateNames.end()) {
      throw SyntaxException(lineNo + 1, "Initial state " + initialName + " not in state set.");
    }

    //Read alphabet
    ++lineNo;
    std::list<std::string> letterNames = this->readNamesIntoList(lines, lineNo);

    //Read transitions
    ++lineNo;
    std::list<std::string> transitionTriplets = this->readNamesIntoList(lines, lineNo);

    //Build state/letter vectors and get initial number
    std::vector<std::string> stateVector(stateNames.begin(), stateNames.end());
    std::vector<std::string> letterVector(letterNames.begin(), letterNames.end());
    int initialState = dasdull::vectorPos(stateVector, initialName);

    //Read acceptance component and build automaton
    if (acceptanceMode == "buechi") {
      ++lineNo;
      this->checkReadTillEnd(lineNo, lines.size());
      //Read final states
      std::list<std::string> finalNames = this->readNamesIntoList(lines, lineNo);
      //Construct final state bit vector
      std::vector<bool> finalStates(stateNames.size(), false);
      std::list<std::string>::iterator iter;
      for (iter = finalNames.begin(); iter != finalNames.end(); ++iter) {
        int pos = dasdull::vectorPos(stateVector, *iter);
        if (pos != -1) {
          finalStates[pos] = true;
        }
      }
      //Build automaton
      if (deterministic) {
        std::vector<std::vector<int> > transitionTable = this->buildTransitionTable(transitionTriplets, stateVector, letterVector);
        return new DeterministicBuechiAutomaton(stateVector.size(),  stateVector,
                                               letterVector.size(), letterVector,
                                               initialState,        transitionTable,
                                               finalStates);
      }
      else {
        std::vector<std::set<int> > transitionRelation = this->buildTransitionRelation(transitionTriplets, stateVector, letterVector);
        return new NondeterministicBuechiAutomaton(stateVector.size(),  stateVector,
                                                   letterVector.size(), letterVector,
                                                   initialState,        transitionRelation,
                                                   finalStates);
      }
    }
    else {
      //This should never be reached in normal execution as automaton types are checked
      throw SyntaxException(lineNo, "Unexpected Automaton type.");
    }
  }
  
  void IOHandler::writeOmegaSemigroupToStream(OmegaSemigroup const &S, std::ostream& out) {
    return; //TODO
  }
  
  std::istream& IOHandler::safeGetLine(std::istream &in, std::string &str) {
    //Try to read line, throw exception on failure.
    try {
      return std::getline(in, str);
    }
    catch (std::istream::failure const &) {
      throw ReadFailedException();
    }
  }

  void IOHandler::checkReadTillEnd(int lineNo, int lines) {
    if (lineNo >= lines) {
      //We Read till end of file
      throw SyntaxException(lineNo, "Reached end of file while parsing.");
    }
  }

  std::list<std::string> IOHandler::readNamesIntoList(std::vector<std::string> const &lines, int lineNo) {
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
    return result;
  }

  std::vector<std::set<int> > IOHandler::buildTransitionRelation(std::list<std::string> const &lines,
                                                                 std::vector<std::string> const &stateVector,
                                                                 std::vector<std::string> const &letterVector) {
    std::vector<std::set<int> > result(stateVector.size());
    //TODO
    return result;
  }

  std::vector<std::vector<int> > IOHandler::buildTransitionTable(std::list<std::string> const &lines,
                                                                   std::vector<std::string> const &stateVector,
                                                                   std::vector<std::string> const &letterVector) {
      std::vector<std::vector<int> > result(stateVector.size(), std::vector<int>(letterVector.size()));
      //TODO
      return result;
    }

  OmegaAutomaton* IOHandler::readAutomatonFromFile(std::string const inputFileName) {
    //Throw exceptions if an error occurs while reading.
    std::ifstream in;
    in.exceptions(std::ios::failbit | std::ios::badbit);
    //Open the input file.
    try {
      in.open(inputFileName, std::ios::in);
    }
    catch(std::ifstream::failure const &) {
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
      try {
        in.close();
      }
      catch(std::ifstream::failure const &) {
        //If closing the stream causes a second exception it is not reported.
        //The original exception that occurred while reading is thrown.
      }
      throw ex;
    }
    //Close the input file.
    try {
      in.close();
    }
    catch(std::ifstream::failure const &) {
    	throw CloseFailedException(inputFileName);
    }
    return result;
  }
  
  OmegaAutomaton* IOHandler::readAutomatonFromStdin() {
    return this->readAutomatonFromStream(std::cin);
  }
  
  void IOHandler::writeOmegaSemigroupToFile(OmegaSemigroup const &S, std::string const outputFileName) {
    return; //TODO
  }
  
  void IOHandler::writeOmegaSemigroupToStdout(OmegaSemigroup const &S) {
    this->writeOmegaSemigroupToStream(S, std::cout);
  }

}

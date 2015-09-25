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
    std::list<std::string> transitionTriplets = this->readNamesIntoList(lines, lineNo, true);

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
        else {
          //Final state not in state set
          throw SyntaxException(lineNo + 1, "Final state " + *iter + " not in state set.");
        }
      }
      //Build automaton
      if (deterministic) {
        std::vector<std::vector<size_t> > transitionTable = this->buildTransitionTable(transitionTriplets, stateVector, letterVector, transNo);
        return new DeterministicBuechiAutomaton(stateVector,
                                               letterVector,
                                               initialState,
                                               transitionTable,
                                               finalStates);
      }
      else {
        std::vector<std::vector<std::set<size_t> > > transitionRelation = this->buildTransitionRelation(transitionTriplets, stateVector, letterVector, transNo);
        return new NondeterministicBuechiAutomaton(stateVector,
                                                   letterVector,
                                                   initialState,
                                                   transitionRelation,
                                                   finalStates);
      }
    }
    else {
      //This should never be reached in normal execution as automaton types are checked
      throw SyntaxException(lineNo, "Unexpected Automaton type.");
    }
  }
  
  void IOHandler::writeAutomatonToStream(OmegaAutomaton const &A, std::ostream& out) {
    try {
      out << A.description();
    }
    catch(std::ostream::failure const &) {
      throw WriteFailedException();
    }
  }

  void IOHandler::writeOmegaSemigroupToStream(OmegaSemigroup const &S, std::ostream& out) {
    return; //TODO
  }

  void IOHandler::checkReadTillEnd(size_t lineNo, size_t lines) {
    if (lineNo >= lines) {
      //We Read till end of file
      throw SyntaxException(lineNo, "Reached end of file while parsing.");
    }
  }

  std::list<std::string> IOHandler::readNamesIntoList(std::vector<std::string> const &lines, size_t lineNo, bool transitionMode) {
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
    //Special case: put together transitions
    if (!transitionMode) {
      return result;
    }
    else {
      if (result.size() % 3 != 0) {
        throw SyntaxException(lineNo + 1, "Invalid transition format(Might be at a later line");
      }
      std::list<std::string> compressedResult;
      std::list<std::string>::const_iterator iter;
      for(iter = result.begin(); iter != result.end(); ++iter) {
        std::string transition = *iter;
        iter++;
        transition += "," + *iter;
        iter++;
        transition += "," + *iter;
        compressedResult.insert(compressedResult.end(), transition);
      }
      return compressedResult;
    }
  }

  std::vector<std::vector<std::set<size_t> > > IOHandler::buildTransitionRelation(std::list<std::string> const &transitions,
                                                                 std::vector<std::string> const &stateVector,
                                                                 std::vector<std::string> const &letterVector,
                                                                 size_t transNo) {
    std::vector<std::vector<std::set<size_t> > > result(stateVector.size(), std::vector<std::set<size_t> >(letterVector.size()));
    std::list<std::string>::const_iterator iter;
    for (iter = transitions.begin(); iter != transitions.end(); ++iter) {
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
              result[originPos][letterPos].insert(targetPos);
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
    std::list<std::string>::const_iterator iter;
    for (iter = transitions.begin(); iter != transitions.end(); ++iter) {
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
    std::vector<std::vector<bool> >::const_iterator outerIter;
    for (outerIter = isDefined.begin(); outerIter != isDefined.end(); ++outerIter) {
      for (std::vector<bool>::const_iterator innerIter = outerIter->begin(); innerIter != outerIter->end(); ++innerIter) {
        if (*innerIter == false) {
          throw SyntaxException(transNo, "There were undefined transitions.\n(Might be in a later line.)");
        }
      }
    }
    return result;
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
      throw ex;
    }
    //Close the input file.
    out.close();
    if (out.fail()) {
      throw CloseFailedException(outputFileName);
    }
  }

  void IOHandler::writeOmegaSemigroupToFile(OmegaSemigroup const &S, std::string const outputFileName) {
    return; //TODO
  }
  
  void IOHandler::writeOmegaSemigroupToStdout(OmegaSemigroup const &S) {
    this->writeOmegaSemigroupToStream(S, std::cout); //TODO
  }


}

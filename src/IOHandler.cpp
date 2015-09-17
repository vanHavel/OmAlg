#include <iostream>
#include <fstream>
#include <algorithm>

#include "IOHandler.h"
#include "IOHandlerExceptions.h"

namespace omalg {

  IOHandler::IOHandler() {
    std::vector<std::string> acceptanceModes;
    acceptanceModes.push_back("buechi");
    this->possibleModes = acceptanceModes;
  }
  
  OmegaAutomaton* IOHandler::readAutomatonFromStream(std::istream &in) {
    std::string temp;
    std::string acceptanceMode;
    bool deterministic = false;

    int lineNo = 1;
    this->safeGetLine(in, acceptanceMode);
    std::transform(acceptanceMode.begin(), acceptanceMode.end(), acceptanceMode.end(), ::tolower);
    if (find(this->possibleModes.begin(), this->possibleModes.end(), acceptanceMode) == this->possibleModes.end()) {
      std::string errorText = "Expected one of the following acceptance modes:";
      std::vector<std::string>::const_iterator it;
      for(it = this->possibleModes.begin(); it != this->possibleModes.end(); ++it) {
        errorText += " ";
        errorText += *it;
      }
      errorText += ".";
      throw SyntaxException(lineNo, errorText);
    }

    ++lineNo;
    this->safeGetLine(in, temp);
    std::transform(temp.begin(), temp.end(), temp.end(), ::tolower);
    if (temp.compare("deterministic") == 0) {
      deterministic = true;
    }
    else if (temp.compare("nondeterministic") == 0) {
      deterministic = false;
    }
    else {
      throw SyntaxException(lineNo, "Expected 'deterministic' or 'nondeterministic'");
    }

    //TODO
    return 0;
  }
  
  void IOHandler::writeOmegaSemigroupToStream(OmegaSemigroup const &S, std::ostream& out) {
    return; //TODO
  }
  
  void IOHandler::safeGetLine(std::istream &in, std::string &str) {
    try {
      std::getline(in, str);
    }
    catch (std::istream::failure const &) {
      throw ReadFailedException();
    }
  }

  OmegaAutomaton* IOHandler::readAutomatonFromFile(std::string const inputFileName) {
    std::ifstream in;
    in.exceptions(std::ios::failbit | std::ios::badbit);
    try {
      in.open(inputFileName, std::ios::in);
    }
    catch(std::ifstream::failure const &) {
      throw OpenFailedException(inputFileName);
    }
    OmegaAutomaton* result;
    try {
      result = this->readAutomatonFromStream(in);
    }
    catch(FileException const &) {
      throw;
    }
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

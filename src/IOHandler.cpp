#include "IOHandler.h"

namespace omalg {
  
  bool IOHandler::readBuechiAutomatonFromStream(BuechiAutomaton *A, std::istream &in) {
    return true; //TODO
  }
  
  bool IOHandler::writeOmegaSemigroupToStream(OmegaSemigroup* S, std::ostream& out) {
    return true; //TODO
  }
  
  bool IOHandler::readBuechiAutomatonFromFile(BuechiAutomaton* A, std::string inputFileName) {
    return true; //TODO
  }
  
  bool IOHandler::readBuechiAutomatonFromStdin(BuechiAutomaton* A) {
    return true; //TODO
  }
  
  bool IOHandler::writeOmegaSemigroupToFile(OmegaSemigroup* S, std::string outputFileName) {
    return true; //TODO
  }
  
  bool IOHandler::writeOmegaSemigroupToStdout(OmegaSemigroup* S) {
    return true; //TODO
  }

}
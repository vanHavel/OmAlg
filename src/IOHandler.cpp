#include "IOHandler.h"

namespace omalg {
  
  OmegaAutomaton* IOHandler::readAutomatonFromStream(std::istream &in) {
    return 0; //TODO
  }
  
  void IOHandler::writeOmegaSemigroupToStream(OmegaSemigroup const &S, std::ostream& out) {
    return; //TODO
  }
  
  OmegaAutomaton* IOHandler::readAutomatonFromFile(std::string inputFileName) {
    return 0; //TODO
  }
  
  OmegaAutomaton* IOHandler::readAutomatonFromStdin() {
    return this->readAutomatonFromStream(std::cin);
  }
  
  void IOHandler::writeOmegaSemigroupToFile(OmegaSemigroup const &S, std::string outputFileName) {
    return; //TODO
  }
  
  void IOHandler::writeOmegaSemigroupToStdout(OmegaSemigroup const &S) {
    this->writeOmegaSemigroupToStream(S, std::cout);
  }

}
#ifndef OMALG_IO_HANDLER
#define OMALG_IO_HANDLER

#include <string>
#include <vector>
#include <iostream>

#include "OmegaAutomaton.h"
#include "BuechiAutomaton.h"
#include "Semigroup.h"
#include "OmegaSemigroup.h"

namespace omalg {

  class IOHandler {
  private:
	  OmegaAutomaton* readAutomatonFromStream(std::istream &in);
	  
	  void writeOmegaSemigroupToStream(OmegaSemigroup const &S, std::ostream &out);
		  
  public: 
	  OmegaAutomaton* readAutomatonFromFile(std::string inputFileName);
	  OmegaAutomaton* readAutomatonFromStdin();

	  void writeOmegaSemigroupToFile(OmegaSemigroup const &S, std::string outputFileName);
	  void writeOmegaSemigroupToStdout(OmegaSemigroup const &S);
  };

}

#endif
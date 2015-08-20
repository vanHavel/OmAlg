#ifndef OMALG_IO_HANDLER
#define OMALG_IO_HANDLER

#include <string>

#include "OmegaAutomaton.h"
#include "BuechiAutomaton.h"
#include "Semigroup.h"
#include "OmegaSemigroup.h"

namespace omalg {

  class IOHandler {
  public: 
	  OmegaAutomaton readAutomaton(std::string inputFileName);
	  void writeAutomaton(OmegaAutomaton A, std::string outputFileName);
	  
	  Semigroup readSemigroup(std::string inputFileName);
	  void writeSemigroup(Semigroup S, std::string outputFileName);
	  
	  OmegaSemigroup readOmegaSemigroup(std::string inputFileName);
	  void writeOmegaSemigroup(OmegaSemigroup S, std::string outputFileName);
  };

}

#endif
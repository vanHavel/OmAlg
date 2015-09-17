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

    std::vector<std::string> possibleModes;

    IOHandler();
    IOHandler(IOHandler const &) = delete;
    void operator=(IOHandler const &) = delete;

	  OmegaAutomaton* readAutomatonFromStream(std::istream &in);
	  void writeOmegaSemigroupToStream(OmegaSemigroup const &S, std::ostream &out);
	  void safeGetLine(std::istream &in, std::string &str);
		  
  public: 

	  static IOHandler& getInstance() {
	    static IOHandler instance;
	    return instance;
	  }

	  OmegaAutomaton* readAutomatonFromFile(std::string inputFileName);
	  OmegaAutomaton* readAutomatonFromStdin();

	  void writeOmegaSemigroupToFile(OmegaSemigroup const &S, std::string outputFileName);
	  void writeOmegaSemigroupToStdout(OmegaSemigroup const &S);
  };

}

#endif

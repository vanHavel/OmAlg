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
	  bool readAutomatonFromStream(OmegaAutomaton* A, std::istream &in);
	  bool writeAutomatonToStream(OmegaAutomaton* A, std::ostream &out);
          
          bool readBuechiAutomatonFromStream(BuechiAutomaton* A, std::istream &in);
	  
	  bool readSemigroupFromStream(Semigroup* S, std::istream &in);
	  bool writeSemigroupToStream(Semigroup* S, std::ostream &out);
	  
	  bool readOmegaSemigroupFromStream(OmegaSemigroup* S, std::istream &in);
	  bool writeOmegaSemigroupToStream(OmegaSemigroup* S, std::ostream &out);
		  
  public: 
	  bool readAutomatonFromFile(OmegaAutomaton* A, std::string inputFileName);
	  bool readAutomatonFromStdin(OmegaAutomaton* A);
	  bool writeAutomatonToFile(OmegaAutomaton* A, std::string outputFileName);
	  bool writeAutomatonToStdout(OmegaAutomaton* A);
          
          bool readBuechiAutomatonFromFile(BuechiAutomaton* A, std::string inputFileName);
	  bool readBuechiAutomatonFromStdin(BuechiAutomaton* A);
	  
	  bool readSemigroupFromFile(Semigroup* S, std::string inputFileName);
	  bool readSemigroupFromStdin(Semigroup* S);
	  bool writeSemigroupToFile(Semigroup* S, std::string filename);
	  bool writeSemigroupToStdout(Semigroup* S);
	  
	  bool readOmegaSemigroupFromFile(OmegaSemigroup* S, std::string inputFileName);
	  bool readOmegaSemigroupFromStdin(OmegaSemigroup* S);
	  bool writeOmegaSemigroupToFile(OmegaSemigroup* S, std::string outputFileName);
	  bool writeOmegaSemigroupToStdout(OmegaSemigroup* S);
  };

}

#endif
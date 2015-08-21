#ifndef OMALG_IO_HANDLER
#define OMALG_IO_HANDLER

#include <string>
#include <iostream>

#include "OmegaAutomaton.h"
#include "BuechiAutomaton.h"
#include "Semigroup.h"
#include "OmegaSemigroup.h"

namespace omalg {

  class IOHandler {
  private:
	  OmegaAutomaton readAutomatonFromStream(std::istream &in);
	  void writeAutomatonToStream(OmegaAutomaton A, std::ostream &out);
	  
	  Semigroup readSemigroupFromStream(std::istream &in);
	  void writeSemigroupToStream(Semigroup S, std::ostream &out);
	  
	  OmegaSemigroup readOmegaSemigroupFromStream(std::istream &in);
	  void writeOmegaSemigroupToStream(OmegaSemigroup S, std::ostream &out);
		  
  public: 
	  OmegaAutomaton readAutomatonFromFile(std::string inputFileName);
	  OmegaAutomaton readAutomatonFromStdin();
	  void writeAutomatonToFile(OmegaAutomaton A, std::string outputFileName);
	  void writeAutomatonToStdout(OmegaAutomaton A);
	  
	  Semigroup readSemigroupFromFile(std::string inputFileName);
	  Semigroup readSemigroupFromStdin();
	  void writeSemigroupToFile(Semigroup S, std::string filename);
	  void writeSemigroupToStdout(Semigroup S);
	  
	  OmegaSemigroup readOmegaSemigroupFromFile(std::string inputFileName);
	  OmegaSemigroup readOmegaSemigroupFromStdin();
	  void writeOmegaSemigroupToFile(OmegaSemigroup S, std::string outputFileName);
	  void writeOmegaSemigroupToStdout(OmegaSemigroup S);
  };

}

#endif
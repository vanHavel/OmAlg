#ifndef OMALG_IO_HANDLER
#define OMALG_IO_HANDLER

#include <string>
#include <vector>
#include <list>
#include <set>
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
	  void writeAutomatonToStream(OmegaAutomaton const &A, std::ostream &out);

	  void writeOmegaSemigroupToStream(OmegaSemigroup const &S, std::ostream &out);

	  void checkReadTillEnd(size_t lineNo, size_t lines);
	  std::list<std::string> readNamesIntoList(std::vector<std::string> const &lines, size_t lineNo, bool transistionMode = false);

	  std::vector<std::vector<std::set<size_t> > > buildTransitionRelation(std::list<std::string> const &transitions,
	                                                                 std::vector<std::string> const &stateVector,
	                                                                 std::vector<std::string> const &letterVector,
	                                                                 size_t transNo);
	  std::vector<std::vector<size_t> > buildTransitionTable(std::list<std::string> const &transitions,
	                                                                 std::vector<std::string> const &stateVector,
	                                                                 std::vector<std::string> const &letterVector,
	                                                                 size_t transNo);
		  
  public: 

	  /**
	   * Getter for the singleton instance of IOHandler.
	   * @return The unique IOHandler instance.
	   */
	  static IOHandler& getInstance() {
	    static IOHandler instance;
	    return instance;
	  }

	  /**
	   * Reads omega automaton from a text file in the omalg format.
	   * @param inputFileName The name of the input file.
	   * @return A pointer to the read automaton.
	   * @throws OpenFailedException if opening the file fails.
	   * @throws CloseFailedException if closing the file fails, but no exception occurred before while reading.
	   * @throws SyntaxException if a syntax error is encountered while reading the file.
	   * @throws ReadFailedException if an error occurs while reading a line from the file.
	   */
	  OmegaAutomaton* readAutomatonFromFile(std::string inputFileName);

	  /**
     * Reads omega automaton from stdin in the omalg format.
     * @return A pointer to the read automaton.
     * @throws SyntaxException if a syntax error is encountered while reading the file.
     * @throws ReadFailedException if an error occurs while reading a line from the file.
     */
	  OmegaAutomaton* readAutomatonFromStdin();

	  void writeAutomatonToStdout(OmegaAutomaton const &A);
	  void writeAutomatonToFile(OmegaAutomaton const &A, std::string outputFileName);

	  void writeOmegaSemigroupToFile(OmegaSemigroup const &S, std::string outputFileName);
	  void writeOmegaSemigroupToStdout(OmegaSemigroup const &S);
  };

}

#endif

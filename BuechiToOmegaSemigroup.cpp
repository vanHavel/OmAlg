#include <string>

#include "OmegaSemigroup.h"
#include "OmegaAutomaton.h"
#include "BuechiAutomaton.h"
#include "OptParser.h"
#include "IOHandler.h"

int main(int argc, char const *argv[]) {
  
  Kanedo::OptParser* optParser = new Kanedo::OptParser();
  optParser->setHelpText("Translation from Buechi automata to omega-semigroups");
  optParser->setOption("input_file", "i", "input-file", "Path to input file for automaton");
  optParser->setOption("output_file", "o", "output-file", "Path to output file for omega-semigroup");
  optParser->addFlag("help", "h", "help", "Print usage text");
  optParser->addFlag("suppress_output", "s", "suppress-output", "Suppress output of warnings by b2os");
  
  if(!optParser->parseOptions(argc, argv)) {
    delete optParser;
    return EXIT_FAILURE;
  }
  
  if(optParser->isSet("help")) {
    optParser->showUsage();
    std::cout << "Consult the manual for additional help and examples.";
    std::cout << std::endl;
    delete optParser;
    return 0;
  }
  
  bool suppressWarnings = optParser->isSet("suppress_output");
  
  omalg::IOHandler* ioHandler = new omalg::IOHandler;
  
  std::string inputFile = optParser->getValue("input_file");
  omalg::BuechiAutomaton* A;
  if(inputFile.empty()) {
    if(!suppressWarnings) {
      std::cerr << "Warning: No input file specified. Reading from stdin. (Type b2os -h for help)";
      std::cerr << std::endl;
    }
    if(!ioHandler->readBuechiAutomatonFromStdin(A)) {
      std::cerr << "Error: failed to read Buechi automaton from stdin";
      std::cerr << std::endl;
      delete optParser;
      delete ioHandler;
      return EXIT_FAILURE;
    }
  }
  else {
    if(!ioHandler->readBuechiAutomatonFromFile(A, inputFile)) {
      std::cerr << "Error: failed to read Buechi automaton from file " << inputFile;
      std::cerr << std::endl;
      delete optParser;
      delete ioHandler;
      return EXIT_FAILURE;
    }
  }
  
  omalg::OmegaSemigroup* B = A->toOmegaSemigroup();
  
  std::string outputFile = optParser->getValue("output_file");
  if(outputFile.empty()) {
    if(!suppressWarnings) {
      std::cerr << "Warning: No output file specified. Writing to stdout. (Type b2os -h for help)";
      std::cerr << std::endl;
      if(!ioHandler->writeOmegaSemigroupToStdout(B)) {
        std::cerr << "Error: failed to write omega semigroup to stdout";
        std::cerr << std::endl;
        delete optParser;
        delete ioHandler;
        delete A;
        return EXIT_FAILURE;
      }
    }
  }
  else {
    if(!ioHandler->writeOmegaSemigroupToFile(B, outputFile)) {
        std::cerr << "Error: failed to write omega semigroup to file " << outputFile;
        std::cerr << std::endl;
        delete optParser;
        delete ioHandler;
        delete A;
        return EXIT_FAILURE;
      }
  }
  
  delete optParser;
  delete ioHandler;
  delete A;
  delete B;
  return 0;
}
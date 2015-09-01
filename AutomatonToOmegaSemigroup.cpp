#include <string>

#include "OmegaSemigroup.h"
#include "OmegaAutomaton.h"
#include "OptParser.h"
#include "IOHandler.h"

int main(int argc, char const *argv[]) {
  
  class E {}; //TODO
  
  Kanedo::OptParser optParser = Kanedo::OptParser();
  optParser.setHelpText("Translation from omega automata to omega-semigroups");
  optParser.setOption("input_file", "i", "input-file", "Path to input file for automaton");
  optParser.setOption("output_file", "o", "output-file", "Path to output file for omega-semigroup");
  optParser.addFlag("help", "h", "help", "Print usage text");
  optParser.addFlag("suppress_output", "s", "suppress-output", "Suppress output of warnings by a2os");
  
  try {
    optParser.parseOptions(argc, argv);
  }
  catch(const Kanedo::UnknownOptionException &E) {
    std::cerr << "Error: Unknown option: " << E.getOptionName() << std::endl;;
    std::cerr << "Type a2os -h for help" << std::endl;
    return EXIT_FAILURE;
  }
  
  if(optParser.isSet("help")) {
    optParser.showUsage();
    std::cout << "Consult the manual for additional help and examples.";
    std::cout << std::endl;
    return 0;
  }
  
  bool suppressWarnings = optParser.isSet("suppress_output");
  
  omalg::IOHandler ioHandler = omalg::IOHandler();
  omalg::OmegaAutomaton *A;
  
  std::string inputFile = optParser.getValue("input_file");
  if(inputFile.empty()) {
    if(!suppressWarnings) {
      std::cerr << "Warning: No input file specified. Reading from stdin. (Type a2os -h for help)";
      std::cerr << std::endl;
    }
    try {
      A = ioHandler.readAutomatonFromStdin();
    }
    catch(E) { //TODO
      std::cerr << "Error: failed to read automaton from stdin";
      std::cerr << std::endl;
      return EXIT_FAILURE;
    }
  }
  else {
    try {
      A = ioHandler.readAutomatonFromFile(inputFile);
    }
    catch(E) { //TODO
      std::cerr << "Error: failed to read automaton from file " << inputFile;
      std::cerr << std::endl;
      return EXIT_FAILURE;
    }
  }
  
  omalg::OmegaSemigroup* B = A->toOmegaSemigroup();
  delete A;
  
  std::string outputFile = optParser.getValue("output_file");
  if(outputFile.empty()) {
    if(!suppressWarnings) {
      std::cerr << "Warning: No output file specified. Writing to stdout. (Type a2os -h for help)";
      std::cerr << std::endl;
    }
    try {
      ioHandler.writeOmegaSemigroupToStdout(*B);
    }
    catch(E) { //TODO
      std::cerr << "Error: failed to write omega semigroup to stdout";
      std::cerr << std::endl;	
	  delete B;
      return EXIT_FAILURE;
    }
  }
  else {
    try {
      ioHandler.writeOmegaSemigroupToFile(*B, outputFile);
    }
    catch(E) { //TODO
      std::cerr << "Error: failed to write omega semigroup to file " << outputFile;
      std::cerr << std::endl;
	  delete B;
      return EXIT_FAILURE;
    }
  }
	
  delete B;
  return 0;
}
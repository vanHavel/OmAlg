#include <string>
#include <memory>

#include "OmegaSemigroup.h"
#include "OmegaAutomaton.h"
#include "OptParser.h"
#include "IOHandler.h"
#include "IOHandlerExceptions.h"

int main(int argc, char const *argv[]) {

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
  std::unique_ptr<omalg::OmegaAutomaton> A;
  
  std::string inputFile = optParser.getValue("input_file");
  if(inputFile.empty()) {
    if(!suppressWarnings) {
      std::cerr << "Warning: No input file specified. Reading from stdin. (Type a2os -h for help)";
      std::cerr << std::endl;
    }
    try {
      std::unique_ptr<omalg::OmegaAutomaton> temp(omalg::IOHandler::getInstance().readAutomatonFromStdin());
      A = std::move(temp);
    }
    catch(omalg::IOException const &e) {
      std::cerr << "Error: failed to read automaton from stdin";
      std::cerr << std::endl;
      std::cerr << e.what();
      std::cerr << std::endl;
      return EXIT_FAILURE;
    }
  }
  else {
    try {
      std::unique_ptr<omalg::OmegaAutomaton> temp(omalg::IOHandler::getInstance().readAutomatonFromFile(inputFile));
      A = std::move(temp);
    }
    catch(omalg::IOException const &e) {
      std::cerr << "Error: failed to read automaton from file " << inputFile;
      std::cerr << std::endl;
      std::cerr << e.what();
      std::cerr << std::endl;
      return EXIT_FAILURE;
    }
  }
  
  std::unique_ptr<omalg::OmegaSemigroup> B(A->toOmegaSemigroup());
  
  std::string outputFile = optParser.getValue("output_file");
  if(outputFile.empty()) {
    if(!suppressWarnings) {
      std::cerr << "Warning: No output file specified. Writing to stdout. (Type a2os -h for help)";
      std::cerr << std::endl;
    }
    try {
      omalg::IOHandler::getInstance().writeOmegaSemigroupToStdout(*B);
    }
    catch(omalg::IOException const &e) {
      std::cerr << "Error: failed to write omega semigroup to stdout";
      std::cerr << std::endl;	
      std::cerr << e.what();
      std::cerr << std::endl;
      return EXIT_FAILURE;
    }
  }
  else {
    try {
      omalg::IOHandler::getInstance().writeOmegaSemigroupToFile(*B, outputFile);
    }
    catch(omalg::IOException const &e) {
      std::cerr << "Error: failed to write omega semigroup to file " << outputFile;
      std::cerr << std::endl;
      std::cerr << e.what();
      std::cerr << std::endl;
      return EXIT_FAILURE;
    }
  }
	
  return 0;
}

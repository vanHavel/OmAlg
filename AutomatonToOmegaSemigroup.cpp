#include <string>
#include <memory>

#include "OmegaSemigroup.h"
#include "OmegaAutomaton.h"
#include "OptParser.h"
#include "IOHandler.h"
#include "IOHandlerExceptions.h"
#include "GeneralException.h"

/**
 * Reads an omega automaton from an input file, transforms it into an equivalent omega semigroup
 * and writes that to an output file.
 * @param argc The number of command line arguments.
 * @param argv The Command line arguments.
 * @return 0 if and only if successful.
 */
int main(int argc, char const *argv[]) {

  //Creation of OptParser and setup of supported options
  Kanedo::OptParser optParser = Kanedo::OptParser();
  optParser.setHelpText("Translation from omega automata to omega-semigroups");
  optParser.setOption("input_file", "i", "input-file", "Path to input file for automaton");
  optParser.setOption("output_file", "o", "output-file", "Path to output file for omega-semigroup");
  optParser.addFlag("help", "h", "help", "Print usage text");
  optParser.addFlag("suppress_output", "s", "suppress-output", "Suppress output of warnings by a2os");
  
  //Parse the command line options
  try {
    optParser.parseOptions(argc, argv);
  }
  catch(const Kanedo::UnknownOptionException &E) {
    std::cerr << "Error: Unknown option: " << E.getOptionName() << std::endl;;
    std::cerr << "Type a2os -h for help" << std::endl;
    return EXIT_FAILURE;
  }
  
  //Show help text if requested
  if(optParser.isSet("help")) {
    optParser.showUsage();
    std::cout << "Consult the manual for additional help and examples.";
    std::cout << std::endl;
    return 0;
  }
  
  //Determine whether to suppress warnings
  bool suppressWarnings = optParser.isSet("suppress_output");

  //Declare pointer to automaton to be read
  std::unique_ptr<omalg::OmegaAutomaton> A;
  
  //Get input file
  std::string inputFile = optParser.getValue("input_file");

  if(inputFile.empty()) {
    //No input file specified
    if(!suppressWarnings) {
      std::cerr << "Warning: No input file specified. Reading from stdin. (Type a2os -h for help)";
      std::cerr << std::endl;
    }
    //Reading from stdin
    try {
      A = std::unique_ptr<omalg::OmegaAutomaton>(omalg::IOHandler::getInstance().readAutomatonFromStdin());
    }
    catch(omalg::IOException const &e) {
      //An error occurred while reading
      std::cerr << "Error: failed to read automaton from stdin";
      std::cerr << std::endl;
      std::cerr << e.what();
      std::cerr << std::endl;
      return EXIT_FAILURE;
    }
  }
  else {
    //Input file was specified
    try {
      A = std::unique_ptr<omalg::OmegaAutomaton>(omalg::IOHandler::getInstance().readAutomatonFromFile(inputFile));
    }
    catch(omalg::IOException const &e) {
      //An error occurred while reading
      std::cerr << "Error: failed to read automaton from file " << inputFile;
      std::cerr << std::endl;
      std::cerr << e.what();
      std::cerr << std::endl;
      return EXIT_FAILURE;
    }
  }

  //Declare pointer for omega semigroup
  std::unique_ptr<omalg::OmegaSemigroup> B;
  //Transform to omega semigroup
  try {
    B = std::unique_ptr<omalg::OmegaSemigroup>(A->toOmegaSemigroup());
  }
  catch(omalg::NotYetSupportedException const&) {
    std::cerr << "Error: Transformation to omega semigroup not yet implemented for given automaton type.";
    std::cerr << std::endl;
    return EXIT_FAILURE;
  }

  
  //Get output file
  std::string outputFile = optParser.getValue("output_file");

  if(outputFile.empty()) {
    //No output file specified
    if(!suppressWarnings) {
      std::cerr << "Warning: No output file specified. Writing to stdout. (Type a2os -h for help)";
      std::cerr << std::endl;
    }
    //Writing to stdout
    try {
      omalg::IOHandler::getInstance().writeOmegaSemigroupToStdout(*B);
    }
    catch(omalg::IOException const &e) {
      //An error occurred while writing
      std::cerr << "Error: failed to write omega semigroup to stdout";
      std::cerr << std::endl;	
      std::cerr << e.what();
      std::cerr << std::endl;
      return EXIT_FAILURE;
    }
  }
  else {
    //Output file was specified
    try {
      omalg::IOHandler::getInstance().writeOmegaSemigroupToFile(*B, outputFile);
    }
    catch(omalg::IOException const &e) {
      //An error occurred while writing
      std::cerr << "Error: failed to write omega semigroup to file " << outputFile;
      std::cerr << std::endl;
      std::cerr << e.what();
      std::cerr << std::endl;
      return EXIT_FAILURE;
    }
  }
	
  //Successful termination
  return 0;
}

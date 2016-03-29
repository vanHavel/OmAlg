#include <iostream>
#include <fstream>

#include "OmegaSemigroup.h"
#include "IOHandler.h"
#include "IOHandlerExceptions.h"
#include "OptParser.h"
#include "DeterministicBuechiAutomaton.h"

/**
 * Reads an omega semigroup from an input file, and transforms it into an equivalent weak
 * Buechi automaton (if possible).
 * @param argc The number of command line arguments.
 * @param argv The Command line arguments.
 * @return 0 if and only if successful.
 */
int main(int argc, char const* argv[]) {
//Creation of OptParser and setup of supported options
  Kanedo::OptParser optParser = Kanedo::OptParser();
  optParser.setHelpText("Given an omega semigroup, construct a deterministic Buechi automaton "
                            "for its language.");
  optParser.setOption("input_file", "i", "input-file", "Path to input file for omega semigroup");
  optParser.setOption("output_file", "o", "output-file", "Path to output file for resulting automaton");
  optParser.addFlag("help", "h", "help", "Print usage text");
  optParser.addFlag("suppress_output", "s", "suppress-output", "Suppress output of warnings by os2d");

  //Parse the command line options
  try {
    optParser.parseOptions(argc, argv);
  }
  catch (const Kanedo::UnknownOptionException &E) {
    std::cerr << "Error: Unknown option: " << E.getOptionName() << std::endl;;
    std::cerr << "Type os2d -h for help" << std::endl;
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

  //Declare pointer to omega semigroup to be read
  std::unique_ptr<omalg::OmegaSemigroup> S;

  //Get input file
  std::string inputFile = optParser.getValue("input_file");

  if(inputFile.empty()) {
    //No input file specified
    if(!suppressWarnings) {
      std::cerr << "Warning: No input file specified. Reading from stdin. (Type os2d -h for help)";
      std::cerr << std::endl;
    }
    //Reading from stdin
    try {
      S = std::unique_ptr<omalg::OmegaSemigroup>(omalg::IOHandler::getInstance().readOmegaSemigroupFromStdin());
    }
    catch(omalg::IOException const &e) {
      //An error occurred while reading
      std::cerr << "Error: failed to read omega semigroup from stdin";
      std::cerr << std::endl;
      std::cerr << e.what();
      std::cerr << std::endl;
      return EXIT_FAILURE;
    }
  }
  else {
    //Input file was specified
    try {
      S = std::unique_ptr<omalg::OmegaSemigroup>(omalg::IOHandler::getInstance().readOmegaSemigroupFromFile(inputFile));
    }
    catch(omalg::IOException const &e) {
      //An error occurred while reading
      std::cerr << "Error: failed to read omega semigroup from file " << inputFile;
      std::cerr << std::endl;
      std::cerr << e.what();
      std::cerr << std::endl;
      return EXIT_FAILURE;
    }
  }

  //construct result
  try {
    omalg::DeterministicBuechiAutomaton A = S->toDetBuechi();

    //get output file
    std::string outputFile = optParser.getValue("output_file");

    //write to output file
    if (outputFile.empty()) {
      //No output file specified
      if (!suppressWarnings) {
        std::cerr << "Warning: No output file specified. Writing to stdout. (Type os2d -h for help)";
        std::cerr << std::endl;
      }
      std::cout << A.description();
    }
    else {
      std::ofstream out;
      out.open(outputFile, std::ios::out);
      if (!out.good()) {
        std::cerr << "Failed to open file " + outputFile + ".";
        std::cerr << std::endl;
        return EXIT_FAILURE;
      }
      out << A.description();
      out.close();
      if (out.fail()) {
        std::cerr << "Failed to close file " + outputFile + ".";
        std::cerr << std::endl;
      }
    }
    return 0;
  }
  catch (omalg::OperationNotApplicableException const& e) {
    std::cerr << "Given omega semigroup can not be transformed into equivalent det. Buechi automaton." ;
    std::cerr << std::endl;
    std::cerr << "The language is not det. Buechi recognizable.";
    std::cerr << std::endl;
    return EXIT_FAILURE;
  }

}
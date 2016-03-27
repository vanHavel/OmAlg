#include <iostream>
#include <fstream>

#include "OmegaSemigroup.h"
#include "IOHandler.h"
#include "IOHandlerExceptions.h"
#include "OptParser.h"

/**
 * Reads an omega semigroup from an input file, and determines whether its language is
 * det. Buechi/CoBuechi/Weak Buechi/E/A-recognizable.
 * @param argc The number of command line arguments.
 * @param argv The Command line arguments.
 * @return 0 if and only if successful.
 */
int main(int argc, char const* argv[]) {
  //Creation of OptParser and setup of supported options
  Kanedo::OptParser optParser = Kanedo::OptParser();
  optParser.setHelpText("Given an omega semigroup, decide whether its language can be accepted "
                        "by various different kinds of deterministic automata.");
  optParser.setOption("input_file", "i", "input-file", "Path to input file for omega semigroup");
  optParser.setOption("output_file", "o", "output-file", "Path to output file for results");
  optParser.addFlag("help", "h", "help", "Print usage text");
  optParser.addFlag("suppress_output", "s", "suppress-output", "Suppress output of warnings by oslh");

  //Parse the command line options
  try {
    optParser.parseOptions(argc, argv);
  }
  catch (const Kanedo::UnknownOptionException &E) {
    std::cerr << "Error: Unknown option: " << E.getOptionName() << std::endl;;
    std::cerr << "Type oslh -h for help" << std::endl;
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
      std::cerr << "Warning: No input file specified. Reading from stdin. (Type oslh -h for help)";
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

  //build result
  std::string resultString = "";
  resultString += "The language of this omega semigroup is:\n";
  resultString += (S->isDBRecognizable() ? " - " : " - not ") + std::string("deterministic Buechi recognizable\n");
  resultString += (S->isDCRecognizable() ? " - " : " - not ") + std::string("coBuechi recognizable\n");
  resultString += (S->isWBRecognizable() ? " - " : " - not ") + std::string("weak Buechi recognizable\n");
  resultString += (S->isERecognizable() ? " - " : " - not ") + std::string("E recognizable\n");
  resultString += (S->isARecognizable() ? " - " : " - not ") + std::string("A recognizable\n");

  //get output file
  std::string outputFile = optParser.getValue("output_file");

  //write to output file
  if (outputFile.empty()) {
    //No output file specified
    if (!suppressWarnings) {
      std::cerr << "Warning: No output file specified. Writing to stdout. (Type oslh -h for help)";
      std::cerr << std::endl;
    }
    std::cout << resultString;
  }
  else {
    std::ofstream out;
    out.open(outputFile, std::ios::out);
    if (!out.good()) {
      std::cerr << "Failed to open file " + outputFile + ".";
      std::cerr << std::endl;
      return EXIT_FAILURE;
    }
    out << resultString;
    out.close();
    if (out.fail()) {
      std::cerr << "Failed to close file " + outputFile + ".";
      std::cerr << std::endl;
    }
  }
  return 0;
}

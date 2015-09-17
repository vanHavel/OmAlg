/**
 * @file OptParserExceptions.h
 * @author dasDull
 * @brief Exceptions used by OptParser.
 */

#ifndef OPT_PARSER_EXCEPTIONS
#define	OPT_PARSER_EXCEPTIONS

#include <exception>
#include <string>
#include <vector>

namespace Kanedo {
    /**
     * @brief Exception thrown when unspecified command line option is encountered.
     *
     * Exception that is thrown when an unknown command line option is encountered.
     */
  class UnknownOptionException: public std::exception {
  private:
    std::string optionName;
  public:
    /**
     * Constructor.
     * @param unknownOptionName Name of the unknown option.
     */
    UnknownOptionException(std::string const unknownOptionName)
  : optionName(unknownOptionName) {}

    /**
     * Returns the name of the unknown option.
     * @return String containing the name of the unknown option.
     */
    std::string getOptionName() const {
      return this->optionName;
    }

    /**
     * Overrides std::exception what function, returns C string with unknown option.
     * @return C string with content "Unknown Option: <optionName>".
     */
    const char* what() const noexcept override {
      std::string message = "Unknown Option: " + this->optionName;
      return message.c_str();
    }

    /**
     * Destructor. It is declared to throw no exceptions as demanded by the virtual
     * destructor of std::exception.
     */
    ~UnknownOptionException() noexcept { };
  };

  /**
   * @brief Exception thrown when too many options are given.
   *
   * Exception that is thrown when too many options are given.
   * This can be only thrown in strict mode.
   */
  class TooManyOptionsException: public std::exception {
  public:
    /**
     * Destructor. It is declared to throw no exceptions as demanded the by virtual
     * destructor of std::exception.
     */
    ~TooManyOptionsException() noexcept { }
  };

  /**
   * @brief Exception thrown when required options are missing.
   *
   * Exception that is thrown when required options are missing.
   * This can only occur when required options are specified.
   */
  class MissingOptionsException: public std::exception {
  private:
    std::vector<std::string> missing;
  public:
    /**
     * Constructor
     * @param leftRequired Vector of required options not given.
     */
    MissingOptionsException(std::vector<std::string> const leftRequired)
  : missing(leftRequired) {}

    /**
     * Returns the vector of missing option names.
     * @return The vector of missing option names
     */
    std::vector<std::string> getMissingOptions() const {
      return this->missing;
    }

    /**
     * Overrides std::exception what function. Returns C string with missing options.
     * @return C string containing names of missing options.
     */
    const char* what() const noexcept override {
      std::string message = "The following required options where not specified: ";
      for(std::vector<std::string>::const_iterator i = this->missing.begin(); i != this->missing.end(); ++i) {
        message += *i + " ";
      }
      message += ".";
      return message.c_str();
    }

    /**
     * Destructor. It is declared to throw no exceptions as demanded the by virtual
     * destructor of std::exception.
     */
    ~MissingOptionsException() noexcept { }

  };
}
#endif	


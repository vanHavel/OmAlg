#ifndef OPT_PARSER_EXCEPTIONS
#define	OPT_PARSER_EXCEPTIONS

#include <exception>
#include <string>
#include <vector>

namespace Kanedo {
    /**
     * Exception that is thrown when unknown command line option is encountered
     */
    class UnknownOptionException: public std::exception {
    protected:
        std::string optionName;
    public:
        /**
         * constructor
         * @param unknownOptionName string name of the unknown option
         */
        UnknownOptionException(std::string unknownOptionName) 
            : optionName(unknownOptionName) {}
        
        /**
         * returns the name of the unknown option
         * @return string 
         */
        std::string getOptionName() const {
            return this->optionName;
        }
		
		/**
		 * overrides standard what method, returns C string with unknown option
		 * @return char* "Unknown Option: <optionName>"
		 */
		const char* what() const throw() {
			std::string message = "Unkown Option: " + this->optionName;
			return message.c_str();
		}

        /**
         * destructor declared to throw no exceptions as demanded by virtual 
         * destructor of std::exception
         */
        ~UnknownOptionException() throw() { };
    };

    /**
     * Exception that is thrown when too many options are given.
     * This can be only thrown in strict mode
     */
    class TooManyOptionsException: public std::exception { };

    /**
     * Exception that is thrown when required options are missing
     * This can only occur when required options are specified
     */
    class MissingOptionsException: public std::exception {
    protected:
        std::vector<std::string> missing;
    public:
        /**
         * constructor
         * @param required vector<string> vector of required options not given
         */
        MissingOptionsException(std::vector<std::string> leftRequired)
            : missing(leftRequired) {}
        
        /**
         * returns the vector of missing option names
         * @return string 
         */
        std::vector<std::string> getMissingOptions() const {
            return this->missing;
        }
		
		/**
	 	* overrides standard what method, returns C string with unknown option
	 	* @return char* "Unknown Option: <optionName>"
	 	*/
		const char* what() const throw () {
			std::string message = "The following required options where not specified: ";
			for(std::vector<std::string>::const_iterator i = this->missing.begin(); i != this->missing.end(); ++i) {
				message += *i + " ";
			}
			message += ".";
			return message.c_str();
		}

        /**
         * destructor declared to throw no exceptions as demanded by virtual 
         * destructor of std::exception
         */
        ~MissingOptionsException() throw() { }
    };
}
#endif	


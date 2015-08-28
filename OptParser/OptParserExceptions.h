#ifndef OPT_PARSER_EXCEPTIONS_H
#define	OPT_PARSER_EXCEPTIONS_H

#include <exception>
#include <string>
#include <vector>

namespace Kanedo {
    /**
     * Exception that is thrown when unkonwn command line option is encountered
     */
    class UnknownOptionException: public std::exception {
    protected:
        std::string optionName;
    public:
        /**
         * constructor
         * @param unknownOptionName string name of the unknown option
         */
        UnknownOptionException(std::string unknownOptionName) {
            optionName = unknownOptionName;
        }
        /**
         * returns the name of the unknown option
         * @return string 
         */
        std::string getOptionName() const {
            return optionName;
        }

        /**
         * destructor declared to throw no exceptions as demanded by virtual 
         * destructor of std::exception
         */
        ~UnknownOptionException() throw() { };
    };

    /**
     * Exception that is thrown when too many options are given.
     * This can be thrown in strict mode
     */
    class TooManyOptionsException: public std::exception { };

    /**
     * Exception that is thrown required options are missing
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
        MissingOptionsException(std::vector<std::string> required) {
            missing = required;
        }
        /**
         * returns the vector of missing option names
         * @return string 
         */
        std::vector<std::string> getMissingOptions() const {
            return missing;
        }

        /**
         * destructor declared to throw no exceptions as demanded by virtual 
         * destructor of std::exception
         */
        ~MissingOptionsException() throw() { };
    };
}
#endif	


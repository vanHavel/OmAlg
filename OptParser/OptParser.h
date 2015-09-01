/**
 * @file OptParser.h
 * @brief OptParser class.
 *
 * A Class to parse CLI Options in a readable and easy way.
 * inspired by Rubys OptionParser
 *
 * @package   Kanedo::OptParser
 * @author    kanedo <software@kanedo.net>
 * @license   MIT
 * @link      http://kanedo.net
 * @copyright 2014 kanedo
 * @version   1.0.2
 */

#ifndef OPTPARSER_H
#define OPTPARSER_H
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <algorithm>

#include "OptParserExceptions.h"

namespace Kanedo{

	/**
	 * @brief Class to parse command line options.
	 *
	 * Class OptParser.
	 * Use this class to parse CLI Options.
	 * Inspired by Rubys OptionParser class.
	 */
	class OptParser {
	protected:
		struct Option{
			std::string name;
			std::string short_opt;
			std::string long_opt;
			std::string descr;
			bool required;
			std::string default_val;
			bool use_default;
			bool flag;
		};
		std::unordered_map<std::string, std::string> values;
		std::unordered_map<std::string, bool> flag_values;
		std::vector<std::string> required;
		std::unordered_map<std::string, Option> options;
		std::unordered_map<std::string, std::string> defaults;
		std::string help;
		std::string binary_name;

		size_t max_name;
		size_t max_long;
		size_t max_short;

		void	setOption(std::string name, std::string short_opt, std::string long_opt, std::string descr, bool required, std::string default_val, bool use_default);
		
		bool strict;
		std::string getUsageText();
		std::ostream *output;
		
		std::unordered_map<std::string, Option>::iterator findOption(std::string argv);
	public:
		OptParser();
		/**
		 * Constructor.
		 * @param output The output stream for usage text. Default: std::cout.
		 */
		OptParser(std::ostream &output);
		
		/**
		 * Defines the intro text.
		 * This is usually a short description of what the application does.
		 * @param text The intro text.
		 */
		void	setHelpText(std::string text);
		/**
		 * Defines an option.
		 * @param name      The name of the option used in getValue.
		 * @see OptParser::getValue
		 * @param short_opt The short variant of this option (w/o -).
		 * @param long_opt  The long variant of this option (w/o --).
		 * @param descr     A description of what this options is used for.
		 */
		void	setOption(std::string name, std::string short_opt, std::string long_opt, std::string descr);

		/**
		 * Defines an option.
		 * @param name      The name of the option used in getValue.
		 * @see OptParser::getValue
		 * @param short_opt The short variant of this option (w/o -).
		 * @param long_opt  The long variant of this option (w/o --).
		 * @param descr     A description of what this options is used for.
		 * @param required  Set to true if this option is required.
		 */
		void	setOption(std::string name, std::string short_opt, std::string long_opt, std::string descr, bool required);
		/**
		 * Defines an option.
		 * @param name        The name of the option used in getValue.
		 * @see OptParser::getValue
		 * @param short_opt   The short variant of this option (w/o -).
		 * @param long_opt    The long variant of this option (w/o --).
		 * @param descr       A description of what this options is used for.
		 * @param required    Set to true if this option is required.
		 * @param default_val Define a default value for this option (only makes sense if required is set to false).
		 */
		void	setOption(std::string name, std::string short_opt, std::string long_opt, std::string descr, bool required, std::string default_val);

		/**
		 * Defines default value for option later.
		 * @param name  The name of the option.
		 * @param value The default value.
		 */
		void	setDefaultValue(std::string name, std::string value);

		/**
		 * Add a simple flag parameter without values. Just set or not-set.
		 * @param  name        The name of the flag used in isSet.
		 * @see OptParser::isSet
		 * @param  short_opt   The short variant of this flag (w/o -).
		 * @param  long_opt    The long variant of this flag (w/o --).
		 * @param  description A description of what this options is used for.
		 */
		void 	addFlag(std::string name, std::string short_opt, std::string long_opt, std::string descr);

		/**
		 * Returns the value of given option.
		 * If option does not exists it'll return an empty string.
		 * If option is not set and has a default value the default is used.
		 * @param  name Name of the option.
		 * @return Value of given option.
		 */
		std::string	getValue(std::string name);

		/**
		 * Returns true if Flag is set.
		 * @see OptParser::addFlag
		 * @param  name Name of the flag.
		 * @return true if the flag is set.
		 */
		bool	isSet(std::string name);
		
		/**
		 * Returns the Help text.
		 * @return The help text.
		 */
		std::string	getHelpText();

		/**
		 * Shows the usage text to stream defined in constructor.
		 * Default is std::cout.
		 * @see OptParser::OptParser
		 */
		void	showUsage();

		/**
		 * Parses the options (usually parameters of main()).
		 * @throws UnknownOptionException if an option that was not set is encountered.
		 * @throws TooManyOptionsException if too many options are given in strict mode.
		 * @throws MissingOptionsException if required options are missing.
		 * @param argc The size of argv.
		 * @param argv Parameters array.
		 */
		void	parseOptions(int argc, const char* argv[]);
		
	};

}
#endif

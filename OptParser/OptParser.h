/**
 * OptParser class.
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
	 * Class OptParser
	 * use this class to parse CLI Options.
	 * inspired by Rubys OptionParser class
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
		 * constructor
		 * @param ostream& output define output stream for usage text. default std::cout
		 */
		OptParser(std::ostream &output);
		
		/**
		 * define intro text.
		 * usually a short description of what the application does
		 * @param text the intro text
		 */
		void	setHelpText(std::string text);
		/**
		 * define an option
		 * @param string name      the name of the option used in @see OptParser::getValue	
		 * @param string short_opt the short variant of this option (w/o -)
		 * @param string long_opt  the long variant of this option (w/o --)
		 * @param string descr     a description of what this options is used for
		 */
		void	setOption(std::string name, std::string short_opt, std::string long_opt, std::string descr);

		/**
		 * define an option
		 * @param string name      the name of the option used in @see OptParser::getValue	
		 * @param string short_opt the short variant of this option (w/o -)
		 * @param string long_opt  the long variant of this option (w/o --)
		 * @param string descr     a description of what this options is used for
		 * @param bool   required  set to true if this option is required
		 */
		void	setOption(std::string name, std::string short_opt, std::string long_opt, std::string descr, bool required);
		/**
		 * [setOption description]
		 * @param string name        the name of the option used in @see OptParser::getValue
		 * @param string short_opt   the short variant of this option (w/o -)
		 * @param string long_opt    the long variant of this option (w/o --)
		 * @param string descr       a description of what this options is used for
		 * @param bool   required    set to true if this option is required
		 * @param string default_val define a default value for this option (only makes sense if required is set to false)
		 */
		void	setOption(std::string name, std::string short_opt, std::string long_opt, std::string descr, bool required, std::string default_val);

		/**
		 * define default value for option later
		 * @param string name  the name of the option
		 * @param string value the default value
		 */
		void	setDefaultValue(std::string name, std::string value);

		/**
		 * add a simple flag parameter without values. just set or not-set
		 * @param  name        the name of the flag used in @see OptParser::isSet
		 * @param  short_opt   the short variant of this flag (w/o -)
		 * @param  long_opt    the long variant of this flag (w/o --)
		 * @param  description a description of what this options is used for
		 */
		void 	addFlag(std::string name, std::string short_opt, std::string long_opt, std::string descr);

		/**
		 * returns the value of given option. 
		 * If option does not exists it'll return an empty string
		 * If option is not set an has a default value the default is used
		 * @param  string name name of the option
		 * @return string
		 */
		std::string	getValue(std::string name);

		/**
		 * returns true if Flag is set @see OptParser::addFlag
		 * @param  name [description]
		 * @return      [description]
		 */
		bool	isSet(std::string name);
		
		/**
		 * return the Help text
		 * @return string
		 */
		std::string	getHelpText();

		/**
		 * show the usage text to stream defined in @see OptParser::OptParser 
		 * default is std::cout
		 */
		void	showUsage();

		/**
		 * parse the options (usually parameters of main()).
		 * @param  int    argc the size of argv
		 * @param  char** argv parameters array
		 */
		void	parseOptions(int argc, const char* argv[]);
		
	};

}
#endif
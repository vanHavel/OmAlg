#ifndef OMALG_GENERAL_EXCEPTION
#define OMALG_GENERAL_EXCEPTION

#include <exception>
#include <string>

namespace omalg {

  class NotYetSupportedException: public std::exception {
    const char* what() const noexcept override {
          std::string message = "Functionality not yet implemented.";
          return message.c_str();
        }
  };

}

#endif

#ifndef OMALG_IO_HANDLER_EXCEPTIONS
#define	OMALG_IO_HANDLER_EXCEPTIONS

#include <exception>
#include <string>

namespace omalg {

  class IOException: public std::exception { };

  class FileException: public IOException {
  private:
    std::string fileName;
  protected:
    std::string getFileName() const {
      return this->fileName;
    }
  public:
    FileException(std::string const theFileName)
      : fileName(theFileName) {}
  };

  class OpenFailedException: public FileException {
  public:
    OpenFailedException(std::string const theFileName)
	    : FileException(theFileName) {}

    const char* what() const noexcept override {
      std::string message = "Failed to open file: " + this->getFileName();
      return message.c_str();
    }
  };

  class CloseFailedException: public FileException {
  public:
  	CloseFailedException(std::string const theFileName)
  	  : FileException(theFileName) {}

  	const char* what() const noexcept override {
  	  std::string message = "Failed to close file: " + this->getFileName();
  	  return message.c_str();
  	}
  };

  class ReadFailedException: public IOException {
  public:
    const char* what() const noexcept override {
      std::string message = "Error while reading from input stream.";
      return message.c_str();
    }
  };

  class WriteFailedException: public IOException {
    public:
      const char* what() const noexcept override {
        std::string message = "Error while writing to output stream.";
        return message.c_str();
      }
    };

  class SyntaxException: public IOException {
  private:
    int lineNo;
    std::string hintText;
  public:
    SyntaxException(int theLineNo, std::string theHintText)
      : lineNo(theLineNo), hintText(theHintText) {}

    const char* what() const noexcept override {
      std::string message = "Syntax error in line " + std::to_string(this->lineNo) + ".";
      message += '\n';
      message += this->hintText;
      return message.c_str();
    }
  };

}

#endif


#ifndef __EXCEPTIONS_HPP__
#define __EXCEPTIONS_HPP__

#include <exception>
#include <string>
#include <stdexcept>

class OurException : public std::runtime_error {
public:
	OurException(std::string const& arg, std::string const& file, int line)
	: std::runtime_error(arg),
	  msg("At file " + file + ", line " + std::to_string(line) + "\n\t" + arg),
	  exceptionCode(arg)
	{}

	// "throw" guarantees the compiler this function will NOT throw any exceptions
	const char * what() const throw() { return msg.c_str(); }
	const char * error() const throw() { return exceptionCode.c_str(); }

private:
	std::string msg;
	std::string exceptionCode;
};

namespace Exceptions {
	inline constexpr char DIV_0[]("Division by 0");
	inline constexpr char NULLPTR[]("Pointer may not be a nullptr");
	inline constexpr char FILE_EXCEPTION[]("Something went wrong while opening the file");
}

/**
 * Compiler macro
 * __FILE__ and __LINE__ are compiler macros
 *
 * Throws a new instance of `OurException`
 */

#ifndef ERROR
#define ERROR(arg) throw OurException(arg, __FILE__, __LINE__);
#endif

#endif

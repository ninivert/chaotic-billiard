#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include <iostream>
#include <string>

namespace Logger {
	enum Level { DEBUG = 0, INFO = 1, WARNING = 2, ERROR = 3, CRITICAL = 4 };

	extern unsigned int level;

	extern void debug(std::string msg);
	extern void info(std::string msg);
	extern void warning(std::string msg);
	extern void error(std::string msg);
	extern void critical(std::string msg);
}

#endif

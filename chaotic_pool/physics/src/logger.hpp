#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include <iostream>
#include <string>

namespace Logger {
	enum Level { DEBUG = 0, INFO = 1, WARNING = 2, ERROR = 3, CRITICAL = 4 };

	unsigned int level(INFO);

	void debug(std::string msg) {
		if (level <= DEBUG)
			std::cout << "[DEBUG] " << msg << std::endl;
	}
	void info(std::string msg) {
		if (level <= INFO)
			std::cout << "[INFO] " << msg << std::endl;
	}
	void warning(std::string msg) {
		if (level <= WARNING)
			std::cout << "[WARNING] " << msg << std::endl;
	}
	void error(std::string msg) {
		if (level <= ERROR)
			std::cout << "[ERROR] " << msg << std::endl;
	}
	void critical(std::string msg) {
		if (level <= CRITICAL)
			std::cout << "[CRITICAL] " << msg << std::endl;
	}
}

#endif

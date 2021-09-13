#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include <iostream>
#include <string>

namespace Logger {
	void info(std::string msg) {
		std::cout << "[INFO] " << msg << std::endl;
	}

	void debug(std::string msg) {
		std::cout << "[DEBUG] " << msg << std::endl;
	}
}

#endif

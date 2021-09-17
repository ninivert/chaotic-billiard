#include "logger.hpp"

unsigned int Logger::level = Logger::INFO;

extern void Logger::debug(std::string msg) {
	if (level <= DEBUG)
		std::cout << "[DEBUG] " << msg << std::endl;
}
extern void Logger::info(std::string msg) {
	if (level <= INFO)
		std::cout << "[INFO] " << msg << std::endl;
}
extern void Logger::warning(std::string msg) {
	if (level <= WARNING)
		std::cout << "[WARNING] " << msg << std::endl;
}
extern void Logger::error(std::string msg) {
	if (level <= ERROR)
		std::cout << "[ERROR] " << msg << std::endl;
}
extern void Logger::critical(std::string msg) {
	if (level <= CRITICAL)
		std::cout << "[CRITICAL] " << msg << std::endl;
}

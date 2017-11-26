
#include "logger.hpp"


/*
#include <time.h>

//YYYY-MM-DD HH:mm:ss
std::string getTimeStamp() {
	time_t now = time(NULL);
	struct tm tstruct;
	char buf[80];
	localtime_s(&tstruct, &now);
	strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	return buf;
}
*/

Logger::Logger() {
	enabled = true;
	type = "file";
	filename = "log.txt";
	filestream.open(filename.c_str(), std::ios::out | std::ios::app);
}

Logger::Logger(std::string fileLog) {
	enabled = true;
	type = "file";
	filename = fileLog;
	filestream.open(filename.c_str(), std::ios::out | std::ios::app);
}

Logger::~Logger() {
	enabled = false;
}

void Logger::log(const std::string& message) {
	filestream << message << std::endl;
}

void Logger::logf(const std::string& f, ...) {
	const char* format = f.c_str();
	va_list args;
	va_start(args, f);

	char buffer[256];
	vsnprintf(buffer, 256, format, args);
	filestream << buffer << std::endl;

	va_end(args);
}

Logger logger;

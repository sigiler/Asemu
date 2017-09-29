
#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <cstdarg>

class Logger {

private:
	bool enabled;
	std::string type;
	std::string filename;
	std::ofstream filestream;

public:
	Logger();
	Logger(std::string fileLog);
	~Logger();
    void log(const std::string& message);
    void logf(const std::string& f, ...);

};

// make logger global
extern Logger logger;

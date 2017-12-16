
#pragma once

#include <string>

struct Config {

	Config();
	virtual ~Config();

	virtual void resetDefaults();

	virtual std::string get(std::string setting);
	virtual void set(std::string setting, std::string value);

	virtual void save(std::string file);
	virtual void load(std::string file);
};

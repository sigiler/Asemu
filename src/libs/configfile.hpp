
#pragma once

#include <map>

struct configs {

	std::map<std::string, std::string> entries;

	std::string configFileName;

	// default
	// set
	// get

	void Save();
	void Load();

};

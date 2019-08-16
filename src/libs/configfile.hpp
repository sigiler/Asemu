
#pragma once

#include <map>

enum choiceSaveLoad : bool {
	save = false,
	load = true,
};

struct configs {

	std::map<std::string, std::string> entries;

	std::string configFileName;

	// default
	void DefaultEntries();

	void setEntry(std::string entryKey, std::string changedValue);
	std::string getEntry(std::string entryKey);

	void chgEntry(std::string entryKey, std::string &valueStr, choiceSaveLoad choice);

	void SaveFile(std::string filepath);
	void LoadFile(std::string filepath);

};

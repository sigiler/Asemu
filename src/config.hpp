
#pragma once

struct Config {

	Config();
	virtual ~Config();

	virtual void resetDefaults();
	virtual void get();
	virtual void set();
	virtual void load();
	virtual void save();
};

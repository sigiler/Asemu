
#pragma once

#include <vector>
#include <string>

class Profiler {

private:
	bool enabled;
	std::vector<int> timings;
	std::vector<int> measures;

public:
	Profiler();
	~Profiler();

	int clock_start();
	int clock_start(std::string category);
	void clock_end(int id);

	int ruler_start();
	int ruler_start(std::string category);
	void ruler_end(int id);

	std::string stats();

};

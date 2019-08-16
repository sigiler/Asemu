
#pragma once

#include <vector>
#include <string>
#include <chrono>

std::chrono::high_resolution_clock clocky;

using time_high_res_ns = std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds>;
using id_index = uint64_t;

class Profiler {

private:
	bool enabled;
	std::vector<int> timings;
	std::vector<int> measures;

	id_index timeIndex;
	id_index spaceIndex;

public:
	Profiler();
	~Profiler();

	id_index clock_start();
	id_index clock_start(std::string category);
	void clock_end(id_index id);

	id_index ruler_start();
	id_index ruler_start(std::string category);
	void ruler_end(id_index id);

	std::string stats();

};


#include "profiler.hpp"

id_index Profiler::clock_start() {
    time_high_res_ns timeStart = clocky.now();

    timings[timeIndex] = timeStart.time_since_epoch().count();
    return ++timeIndex;
}

void Profiler::clock_end(id_index id) {
    time_high_res_ns timeEnd = clocky.now();

    timings[id+1] = timeEnd.time_since_epoch().count();
}

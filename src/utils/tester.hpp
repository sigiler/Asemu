
#pragma once

#include <functional>

class Tester {

private:
	void* tests;

public:
	void addTest(std::function<void* (...)>);
	void runAll();

};

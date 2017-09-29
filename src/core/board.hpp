#pragma once

#include <vector>
#include <string>

#include "cpu.hpp"
#include "keys.hpp"
#include "mem.hpp"
#include "ports.hpp"
#include "screen.hpp"
#include "speaker.hpp"
#include "timer.hpp"

enum ComponentType {
	COMP_CPU, COMP_MEM, COMP_MMU, COMP_PORTS, COMP_KEYS, COMP_SCREEEN,
};

// the C way

struct component {
	u8 id;
	u8 type;
	union {
		cpu* c;
		memory* m;
		ports* p;

		keys* k;
		screen* s;
	};
};

struct board {
	u8 id;
	u8 type;
	u8 number;
	component* components;
};

// the C++ way way

class componentpp {
	u8 id;
	u8 type;
	std::string name;
};
// components inherit this

class boardpp : componentpp {
	std::vector<componentpp> parts;
	// connections between them?
};

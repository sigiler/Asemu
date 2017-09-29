
#include "emu.hpp"


Emu::Emu() {
	id = 0;
	running = false;
	m = new memory;
	c = new cpu;
	s = new screen;
	a = new speaker;
	k = new keys;
	t = new timer;
}

Emu::~Emu() {
	id = 0;
	running = false;
	delete c;
	delete s;
	delete a;
	delete k;
	delete m;
	delete t;
	c = nullptr;
	s = nullptr;
	a = nullptr;
	k = nullptr;
	m = nullptr;
	t = nullptr;
}


void Emu::Init() {
	// better reset cpu
	c->reset();
	// connect memory to cpu, screen, speaker, keyboard and timer
	c->mem = m;
	s->mem = m;
	a->mem = m;
	k->mem = m;
	t->mem = m;
}

void Emu::DeInit() {
	c->reset();
}

void Emu::Advance(u32 steps) {
	// steps are time units (nanoseconds or cycles, whatever)
	c->advance(steps);
	t->update_advance(steps);
}

void Emu::loadState(std::string stateFileName) {

	if (stateFileName.length() == 0) {
		std::ifstream filestream;
		filestream.open(stateFileName.c_str(), std::ios::in);
		// TODO save emu state
	}

}

void Emu::saveState(std::string stateFileName) {

	if (stateFileName.size() == 0) {
		std::ofstream filestream;
		filestream.open(stateFileName.c_str(), std::ios::out);
		// TODO load emu state
	}

}

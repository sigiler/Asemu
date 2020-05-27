
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
	// reset cpu
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

	if (stateFileName.length() > 0) {
		std::ifstream filestream;
		filestream.open(stateFileName.c_str(), std::ios::in | std::ios::binary);
		std::vector<u8> state;

		state.resize(16*1024*1024);
		filestream.read((char*)state.data(), state.size());
		m->load(state);

		state.resize(3*12);
		filestream.read((char*)state.data(), state.size());
		c->load(state);
	}

}

void Emu::saveState(std::string stateFileName) {

	if (stateFileName.size() > 0) {
		std::ofstream filestream;
		filestream.open(stateFileName.c_str(), std::ios::out | std::ios::binary);

		std::vector<u8> state;

		state.resize(16*1024*1024);
		m->save(state);
		filestream.write((const char*)state.data(), state.size());

		state.resize(3*12);
		c->save(state);
		filestream.write((const char*)state.data(), state.size());
	}

}

void Emu::loadRom(std::string filename) {
	std::fstream filestream;
	std::streampos filesize;
	char* memblock;
	u32 memsize;

	// read rom/image binary file
	filestream.open(filename.c_str(), std::ios::in | std::ios::binary);
	if (!filestream.is_open()) {
		return;
	}
	filestream.seekg(0, std::ios::end);
    filesize = filestream.tellg();
	memsize = (u32) filesize;
    memblock = new char[memsize];
    filestream.seekg(0, std::ios::beg);
    filestream.read(memblock, filesize);
    filestream.close();
	
	// copy rom into memory
	for (u32 i = 0; i < memsize; i++) {
		this->m->buffer[i] = memblock[i];
	}

	delete[] memblock;
}

void Emu::saveRom(std::string filename) {
	// TODO
}

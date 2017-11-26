
#include "mem.hpp"

memory::memory() {
	type = M_SIMPLE;
	rights = M_ALL;
	size = 0x1000000;
	buffer = new u8[size];
}

memory::memory(u64 s) {
	type = M_SIMPLE;
	rights = M_ALL;
	size = s;
	buffer = new u8[s];
}

memory::~memory() {
	type = 0;
	rights = 0;
	size = 0;
	delete[] buffer;
}

u8 memory::read_byte(u32 address) {
	return buffer[address];
}

void memory::write_byte(u32 address, u8 byte) {
	buffer[address] = byte;
}

void memory::save(std::vector<u8> buf) {
	buf.reserve(size);
	for (u32 i = 0; i < size; i++) {
		buf[i] = buffer[i];
	}
}

void memory::load(std::vector<u8> buf) {
	buf.reserve(size);
	for (u32 i = 0; i < size; i++) {
		buffer[i] = buf[i];
	}
}

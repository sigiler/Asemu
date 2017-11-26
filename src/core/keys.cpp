
#include "keys.hpp"

keys::keys() {
	type = 0;
	number = 64;
	state = new bool[number];

	mem = nullptr;
	base_address = 0xBB0000;
	port = nullptr;
	port_index = 0xBB;
}

keys::~keys() {
	number = 0;
	base_address = 0;
	mem = nullptr;

	delete[] state;
	state = nullptr;
}

void keys::onPress(u8 keyCode) {
	if (keyCode >= number) {
		return;
	}
	state[keyCode] = 1;
	mem->buffer[base_address + keyCode] = 1;
}

void keys::onRelease(u8 keyCode) {
	if (keyCode >= number) {
		return;
	}
	state[keyCode] = 0;
	mem->buffer[base_address + keyCode] = 0;
}

void keys::onUpdate(u8* keyStates) {
	for (u8 i = 0; i < number; i++) {
		state[i] = keyStates[i];
		mem->buffer[base_address + i] = keyStates[i];
	}
}

void keys::onUpdateRow(u8 row, u32 row_state) {
	mem->buffer[base_address + 3*row] = (row_state >> 16) & 0xFF;
	mem->buffer[base_address + 3*row + 1] = (row_state >> 8) & 0xFF;
	mem->buffer[base_address + 3*row + 2] = (row_state >> 0) & 0xFF;
}

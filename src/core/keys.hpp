
#pragma once

#include "mem.hpp"
#include "ports.hpp"

enum KEYS {
	// arrows
	KB_LEFT, KB_RIGHT,
	KB_UP, KB_DOWN,
	// console keys
	BUTTON_A, BUTTON_B,
	BUTTON_X, BUTTON_Y,
	BUTTON_L, BUTTON_R,
	// phone keys
	KEY_T0, KEY_T1, KEY_T2,
	KEY_1, KEY_2, KEY_3,
	KEY_4, KEY_5, KEY_6,
	KEY_7, KEY_8, KEY_9,
	KEY_B0, KEY_0, KEY_B1,
	// QWERTY keyboard
	// https://en.wikipedia.org/wiki/British_and_American_keyboards
	// each row goes to a byte, 6*3=18 bytes
	K_Q, K_W,
	K_A, K_S,
	K_Z, K_X,

};

struct keys {

	keys();
	~keys();

	// properties
	u8 type;    // console like, gamepad, qwerty keyboard
	u8 number;  // number of keys

	// internal state
	bool* state;  // 1 for pressed, 0 otherwise

	// memory mapped
	memory* mem;
	u32 base_address;
	// port
	ports* port;
	u32 port_index;

	void onPress(u8 keyCode);
	void onRelease(u8 keyCode);
	void onUpdate(u8* keyStates);
	void onUpdateRow(u8 row, u32 row_state);

	void save(u8* buf);
	void load(u8* buf);

};

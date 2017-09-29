
#include "sdl_input.hpp"

// TODO configurable key mapping
//u8 keyMapping[256];

void updateKeyboard(keys* k) {

	const Uint8* keyboard_state;

	keyboard_state =  SDL_GetKeyboardState(NULL);

	uint8_t relevant_key[7][24] = {
			{SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_RETURN, SDL_SCANCODE_BACKSPACE,},
			{0, SDL_SCANCODE_F1, SDL_SCANCODE_F2, SDL_SCANCODE_F3, SDL_SCANCODE_F4, SDL_SCANCODE_F5, SDL_SCANCODE_F6, SDL_SCANCODE_F7, SDL_SCANCODE_F8, SDL_SCANCODE_F9, SDL_SCANCODE_F10,},
			{0, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4, SDL_SCANCODE_5, SDL_SCANCODE_6, SDL_SCANCODE_7, SDL_SCANCODE_8, SDL_SCANCODE_9, SDL_SCANCODE_0,},
			{0, SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_R, SDL_SCANCODE_T, SDL_SCANCODE_Y, SDL_SCANCODE_U, SDL_SCANCODE_I, SDL_SCANCODE_O, SDL_SCANCODE_P,},
			{0, SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_F, SDL_SCANCODE_G, SDL_SCANCODE_H, SDL_SCANCODE_J, SDL_SCANCODE_K, SDL_SCANCODE_L,},
			{0, SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_V, SDL_SCANCODE_B, SDL_SCANCODE_N, SDL_SCANCODE_M,},
			{0, 0, 0, SDL_SCANCODE_SPACE, 0, 0, 0,},
	};
	//u8 row_sizes[] = {6, 11, 11, 9, 7, 7};

	u32 keys_row_state;
	u32 key_scancode;
	u32 key_bit;
	u8 row, col;
	for (row = 0; row < 7; row++) {
		keys_row_state = 0;
		key_bit = 1 << 24;
		for (col = 0; col < 24; col++) {
			key_scancode = relevant_key[row][col];
			if (keyboard_state[key_scancode]) {
				keys_row_state |= key_bit;
			}
			key_bit >>= 1;
		}
		k->onUpdateRow(row, keys_row_state);
	}

}

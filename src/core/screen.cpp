
#include "screen.hpp"

screen::screen() {
	width = 320;
	height = 240;

	depth = 24;

	color_mode = 0;
	palette = nullptr;

	framerate = 24;
	
	pixel_data = nullptr;

	mem = nullptr;
	base_address = 0x5C0000;

	port = nullptr;
	port_index = 0x00005C;
}

screen::~screen() {
	width = 0;
	height = 0;
	depth = 0;

	pixel_data = nullptr;

	mem = nullptr;
	base_address = 0;

	port = nullptr;
	port_index = 0;
}

u8* screen::getPixelBuffer() {
	return &mem->buffer[base_address];
}

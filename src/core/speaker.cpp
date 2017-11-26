

#include "speaker.hpp"

speaker::speaker() {
	bit_depth = 24;
	sample_rate = 48000;

	channels = 3;

	samples = nullptr;
	muted = 1;
	volume = 128;

	mem = nullptr;
	base_address = 0xA00000;

	port = nullptr;
	port_index = 0xA0;
}

speaker::~speaker() {

}

u8* speaker::getSamplesBuffer() {
	return &mem->buffer[base_address];
}


#pragma once

#include "mem.hpp"
#include "ports.hpp"

struct speaker {

	speaker();
	~speaker();

	// properties
	u32 bit_depth;    // bits per sample
	u32 sample_rate;  // frequency
	u8 channels;      // number of channels

	// internal state
	u8* samples;
	u8 muted;
	u8 volume;

	// memory mapped io
	memory* mem;
	u32 base_address;
	// port io
	ports* port;
	u32 port_index;

	u8* getSamplesBuffer();
	f32* convertSampleToFloat(u8*);

	void save(u8* buf); // aka serialize
	void load(u8* buf); // aka unserialize

};

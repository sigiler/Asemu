
#pragma once

#include "mem.hpp"
#include "ports.hpp"


struct screen {

	screen();
	~screen();

	// properties
	u32 width;
	u32 height;
	u32 depth;   // bits per pixel

	u8 color_mode; // BW, GS, color palette, RGB, RGBA
	u8* palette; // table of colours

	u8 framerate;

	// internal state
	u8* pixel_data;

	// memory mapped io
	memory* mem;
	u32 base_address; // starting memory address for pixel data
	// port io
	ports* port;
	u32 port_index;

	// used by frontend, frontend will deal with being in RGB888
	u8* getPixelBuffer();

	void save(u8* buf); // aka serialize
	void load(u8* buf); // aka unserialize

};

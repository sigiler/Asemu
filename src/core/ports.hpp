
#pragma once

#include "../common/common_types.hpp"

struct ports {

	u8 size;

	typedef void (*portInByteCallback)(u8);
	typedef void (*portInWordCallback)(u32);
	typedef u8 (*portOutByteCallback)();
	typedef u32 (*portOutWordCallback)();
	portInByteCallback* byteIn;
	portOutByteCallback* byteOut;
	portInWordCallback* wordIn;
	portOutWordCallback* wordOut;

	ports();
	~ports();

	void in_byte(u8 p, u8 v);
	u8 out_byte(u8 p);

	void in_word(u8 p, u32 w);
	u32 out_word(u8 p);

};

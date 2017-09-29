
#include "ports.hpp"

ports::ports() {
	size = 24;
	byteIn = new portInByteCallback[size];
	byteOut = new portOutByteCallback[size];
	wordIn = new portInWordCallback[size];
	wordOut = new portOutWordCallback[size];
}

ports::~ports() {
	delete[] byteIn;
	delete[] byteOut;
	delete[] wordIn;
	delete[] wordOut;
}

void ports::in_byte(u8 p, u8 b) {
	byteIn[p](b);
}

u8 ports::out_byte(u8 p) {
	return byteOut[p]();
}

void ports::in_word(u8 p, u32 w) {
	wordIn[p](w);
}

u32 ports::out_word(u8 p) {
	return wordOut[p]();
}

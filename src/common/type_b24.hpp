
#pragma once

#include "type_u24.hpp"

// u24 vector accessor

// bit field u24
// bit access to u24

template <u8 b> struct b24 {

private:
	u24 val;
	enum {bit = b};
	enum {mask = 1 << b};

public:
	inline b24() = default;
	inline operator bool() const {
		return (bool)(val & mask);
	}
	inline b24& operator=(const bool value) {
		val = (val & ~mask) | (value << bit);
		return *this;
	}

};

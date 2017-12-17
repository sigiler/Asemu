
#pragma once

#include "type_u24.hpp"

// access bit in u24

template <u8 b> struct b24 {

private:
	u24 val;
	enum {bit = b};
	enum {mask = 1 << b};

	inline bool get() { return (val & mask); }
	inline void set(bool v) { val = (val & ~mask) | (v << bit); }

public:
	inline b24() = default;
	inline b24(const b24& other) { set(other->get()); }
	inline b24(const bool v) { set(v); }

	inline operator bool() { return get(); }

	inline b24& operator=(const b24& other) { set(other->get()); return *this; }
	inline b24& operator=(const bool v) { set(v); return *this; }

	inline b24& operator|=(const bool v) { set(v) ; return *this; }
	inline b24& operator&=(const bool v) { set(v) ; return *this; }
	inline b24& operator^=(const bool v) { set(v) ; return *this; }

};

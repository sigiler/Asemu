
#pragma once

#include "type_u24.hpp"

// optimize access to registers instead of a costly reference

// TODO

template <u8 i> struct RegView {

	u24 r[12];

	operator u24() const {
		return r[i];
	}
	operator u32() const {
		return (u32)r[i];
	}

	RegView& operator=(const u24 v) {
		r[i] = v;
		return *this;
	}

	RegView& operator+=(const u24 v) {
		r[i] += v;
		return *this;
	}

	RegView& operator-=(const u24 v) {
		r[i] -= v;
		return *this;
	}

};

template <u8 a, u8 b> struct RegViewRange {

};

template <u8 r, u8 b> struct FlagView {

};


// efficient implementation with nice syntax
struct registersV2 {
    union {
    	u24 r[12];
        RegView<0> a;
        RegView<1> b;
        RegView<2> c;
        RegView<3> x;
        RegView<4> y;
        RegView<5> z;

        // hmm how to put flags here?
        // maybe redo this syntax: fs.f.z, fs.s.i, etc.
        RegView<6> fs;
        RegView<7> pr;
        RegView<8> sp;
        RegView<9> cp;
        RegView<10> ip;
        RegView<11> ep;

        RegViewRange<0, 6> gpr;
        RegViewRange<6, 12> spr;
    };
};

struct RegFS {

	bool v;
	bool n;
	bool z;
	bool c;

	bool i;
	bool e;

	RegFS(const u32& val) {
		v = val & (1 << 3);
		n = val & (1 << 2);
		z = val & (1 << 1);
		c = val & (1 << 0);
		e = 0;
		i = 0;
	}

	inline operator u32() {
		return v << 3 | n << 2 | z << 1 | c << 0;
	}
	inline RegFS& operator=(const u32 val) {
		v = val & (1 << 3);
		n = val & (1 << 2);
		z = val & (1 << 1);
		c = val & (1 << 0);
		return *this;
	}
};


template <u8 index> struct RegFlagView {
private:
	u24 r[12];

public:

	// TODO fix
	struct {
		b24<23> i;
		b24<22> e;
		b24<23> ie;
		b24<22> ee;
	} s;

	struct {
		b24<3> v;
		b24<2> n;
		b24<1> z;
		b24<0> c;
	} f;

	inline operator u24() const { return (u24)r[index]; }
	inline operator u32() const { return (u32)r[index]; }
	inline RegFlagView& operator=(const u24 v) { r[index] = v; return *this; }
	RegFlagView() { r[index] = 0; };
};

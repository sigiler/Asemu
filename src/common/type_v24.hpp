
#pragma once

#include "type_u24.hpp"

// vector of u24

template<size_t size>
struct v24 {
	u24 data[size];
};

// access u24 in vector of u24

template <size_t vecsize, size_t index>
struct u24View {
	u24 vec[vecsize];

	operator u24() const {
		return vec[index];
	}

	u24View& operator=(const u24 val) {
		vec[index] = val;
		return *this;
	}
};


// Asemu specific, move later

template<u8 i> struct RegView {

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

	u24 r[12];

	u24& operator[](u8 ind) {
		assert(ind < b);
		return r[a+ind];
	}

};

template <u8 i, u8 b> struct FlagView {
private:
	u24 v[24];

	enum {bit = b};
	enum {mask = 1 << b};

	inline bool get() { return (v[i] & mask); }
	inline void set(bool bv) { v[i] = (v[i] & ~mask) | (bv << bit); }

public:
	inline FlagView() = default;
	inline FlagView(const FlagView& other) { set(other->get()); }
	inline FlagView(const bool v) { set(v); }

	inline operator bool() { return get(); }

	inline FlagView& operator=(const FlagView& other) { set(other->get()); return *this; }
	inline FlagView& operator=(const bool v) { set(v); return *this; }

	inline FlagView& operator|=(const bool v) { set(v) ; return *this; }
	inline FlagView& operator&=(const bool v) { set(v) ; return *this; }
	inline FlagView& operator^=(const bool v) { set(v) ; return *this; }
};


template <u8 index> struct RegFlagView {

private:
	u24 r[12];

public:
	union {
		FlagView<index, 23> p;   // in privileged mode
		FlagView<index, 22> i;   // in interrupt mode
		FlagView<index, 21> e;   // in exception mode
		FlagView<index, 20> ie;  // interrupts enabled
		FlagView<index, 19> ee;  // exceptions enabled
		FlagView<index, 18> v;   // virtual memory enabled
	} s;

	union {
		FlagView<index, 3> v;  // overflow flag
		FlagView<index, 2> n;  // negative flag
		FlagView<index, 1> z;  // zero flag
		FlagView<index, 0> c;  // carry flag
	} f;

	inline operator u24() const { return (u24)r[index]; }
	inline operator u32() const { return (u32)r[index]; }
	inline RegFlagView& operator=(const u24 v) { r[index] = v; return *this; }
	RegFlagView() : r() {};
};


struct RegFS {

	struct ff {
		bool v;
		bool n;
		bool z;
		bool c;
	} f;

	struct ss {
		bool i;
		bool e;
	} s;

	RegFS() : f(), s() {

	}

	RegFS(const u32& val) {
		f.v = val & (1 << 3);
		f.n = val & (1 << 2);
		f.z = val & (1 << 1);
		f.c = val & (1 << 0);
		s.e = 0;
		s.i = 0;
	}

	inline operator u24() {
		return f.v << 3 | f.n << 2 | f.z << 1 | f.c << 0;
	}

	inline RegFS& operator=(const u32 val) {
		f.v = val & (1 << 3);
		f.n = val & (1 << 2);
		f.z = val & (1 << 1);
		f.c = val & (1 << 0);
		return *this;
	}
};

struct RegFSV2 {

	union {
		u32 data : 24;
		struct {
			u32 v : 1;
			u32 n : 1;
			u32 c : 1;
			u32 z : 1;
		} f;
		struct {
			u32 i : 1;
			u32 e : 1;
		} s;
	};

	RegFSV2() : data() {

	}

	RegFSV2(const u32& val) {
		f.v = val & (1 << 3);
		f.n = val & (1 << 2);
		f.z = val & (1 << 1);
		f.c = val & (1 << 0);
		s.e = 0;
		s.i = 0;
	}

	inline operator u24() {
		return f.v << 3 | f.n << 2 | f.z << 1 | f.c << 0;
	}

	inline RegFSV2& operator=(const u32 val) {
		f.v = val & (1 << 3);
		f.n = val & (1 << 2);
		f.z = val & (1 << 1);
		f.c = val & (1 << 0);
		return *this;
	}
};

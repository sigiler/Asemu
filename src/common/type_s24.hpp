#pragma once

#include "common_types.hpp"

class s24 {

private:
	s32 value :24;

public:
	// constructors
	s24() :
			value() {
	}
	s24(const s24& val) {
		value = val.value;
	}
	s24(const int& val) {
		value = val;
	}

	// conversion needed for operations
	operator int() {
		return value;
	}

	// compound assignments need overloading
	s24& operator +=(s24 that) {
		this->value = this->value + that.value;
		return *this;
	}
	s24& operator -=(s24 that) {
		this->value = this->value - that.value;
		return *this;
	}
	s24& operator *=(s24 that) {
		this->value = this->value * that.value;
		return *this;
	}
	s24& operator /=(s24 that) {
		this->value = this->value / that.value;
		return *this;
	}
	s24& operator %=(s24 that) {
		this->value = this->value % that.value;
		return *this;
	}
	s24& operator &=(s24 that) {
		this->value = this->value & that.value;
		return *this;
	}
	s24& operator |=(s24 that) {
		this->value = this->value | that.value;
		return *this;
	}
	s24& operator ^=(s24 that) {
		this->value = this->value ^ that.value;
		return *this;
	}
	s24& operator >>=(s24 that) {
		this->value = this->value >> that.value;
		return *this;
	}
	s24& operator <<=(s24 that) {
		this->value = this->value << that.value;
		return *this;
	}

};

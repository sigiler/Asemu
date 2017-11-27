#pragma once

#include "common_types.hpp"

class s24 {

private:
	s32 value :24;  // possibly use pack hack in order to be only 3 bytes
	//u8 bytes[3];  // compact version but possibly slower
	//u32 value;    // faster version, more code

public:
	// constructors
	s24() : value() {

	}
	s24(const s24& val) {
		value = val.value;
	}
	s24(const int& val) {
		value = val & 0x00FFFFFF;
	}

	// conversion needed for operations
    operator int() {
		return (int)value;
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

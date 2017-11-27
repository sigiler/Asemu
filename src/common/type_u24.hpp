#pragma once

#include "common_types.hpp"

class u24 {

private:
	u32 value : 24;  // possibly use pack in order to guarantee only 3 bytes
	//u8 bytes[3];   // compact version but slower
	//u32 value;     // faster version, more code

public:
	// constructors
	//u24() : value() {
	//
	//}
	u24() {
		value = 0;
	}
	u24(const u24& val) {
		value = val.value;
	}
	u24(const int& val) {
		value = val & 0x00FFFFFF;
	}

	// conversion needed for operations
    operator int() {
		return (int)value;
	}

    // compound assignments need overloading
    u24& operator +=(u24 that) {
    	this->value = this->value + that.value;
    	return *this;
    }
    u24& operator -=(u24 that) {
    	this->value = this->value - that.value;
    	return *this;
    }
    u24& operator *=(u24 that) {
    	this->value = this->value * that.value;
    	return *this;
    }
    u24& operator /=(u24 that) {
    	this->value = this->value / that.value;
    	return *this;
    }
    u24& operator %=(u24 that) {
    	this->value = this->value % that.value;
    	return *this;
    }
    u24& operator &=(u24 that) {
    	this->value = this->value & that.value;
    	return *this;
    }
    u24& operator |=(u24 that) {
    	this->value = this->value | that.value;
    	return *this;
    }
    u24& operator ^=(u24 that) {
    	this->value = this->value ^ that.value;
    	return *this;
    }
    u24& operator >>=(u24 that) {
    	this->value = this->value >> that.value;
    	return *this;
    }
    u24& operator <<=(u24 that) {
    	this->value = this->value << that.value;
    	return *this;
    }

};

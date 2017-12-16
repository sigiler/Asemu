#pragma once

#include "common_types.hpp"

class u24 {

private:
	// force 3 bytes with pragma pack
	u32 value : 24;

public:
	u24() :value() {};

	u24(const u24& val) {
		value = val.value;
	}
	u24(const int& val) {
		value = val;
	}

    operator u32() const {
    	return value;
	}

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

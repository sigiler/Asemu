
#pragma once

#include <cstdint>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float f32;
typedef double f64;

// unsigned 24 bit integer implementation
class u24 {

private:
    u32 value : 24; // possibly use pack hack in order to be only 3 bytes
    //u8 bytes[3];  // compact version but possibly slower
    //u32 value;    // faster version, more code

public:

    // constructors
    u24() : value() {
    	// undefined
    }
    u24(const u24& val) {
    	value = val.value;
    }
    u24(const int& val) {
    	value = val & 0x00FFFFFF;
    }

    // conversions
    operator int() {
		return (int)value;
	}

    // assignments
    u24& operator = (const u24& val) {
    	this->value = val.value;
        return *this;
    }
    u24& operator = (const int& val) {
        value = val & 0x00FFFFFF;
        return *this;
    }
 	u24& operator *() {
 		return *this;
 	}

    // operators arithmetical
    u24 operator + (u24& that) {
    	return u24(this->value + that.value);
    }
    u24 operator * (u24& that) {
    	return u24(this->value * that.value);
    }
    u24 operator / (u24& that) {
    	return u24(this->value / that.value);
    }
    u24 operator % (u24& that) {
    	return u24(this->value % that.value);
    }
    u24 operator - () {
    	return u24(-(this->value));
    }

    // comparisons
    bool operator == (u24& that) {
    	return this->value == that.value;
    }
    bool operator != (u24& that) {
        	return this->value != that.value;
    }
	bool operator > (u24& that) {
		return this->value > that.value;
	}
	bool operator < (u24& that) {
		return this->value < that.value;
	}
	bool operator >= (u24& that) {
		return this->value >= that.value;
	}
	bool operator <= (u24& that) {
		return this->value <= that.value;
	}

	// operators logical
	bool operator &&(u24& that) {
		return this->value && that.value;
	}
	bool operator || (u24& that) {
		return this->value || that.value;
	}
    bool operator ! () {
    	return this->value == 0;
    }

    // operators bitwise
    u24 operator & (u24& that) {
		return u24(this->value & that.value);
	}
    u24 operator | (u24& that) {
		return u24(this->value | that.value);
	}
    u24 operator ^ (u24& that) {
		return u24(this->value ^ that.value);
    }
    u24 operator << (u24& that) {
		return u24(this->value >> that.value);
	}
    u24 operator >> (u24& that) {
		return u24(this->value >> that.value);
	}

    u24 operator ~ () {
		return u24(~(this->value));
	}

    // compound assignments
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

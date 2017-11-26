
#pragma once

#include "mem.hpp"
#include "ports.hpp"


struct rtc_state {
	// naive but will do
	u16 year;
	u8 month;
	u8 day;
	u8 hour;
	u8 minute;
	u8 second;
	u16 milisecond;
};

struct timer {

	timer();
	~timer();

	// internal state
	u32 frequency[3];
	u8 enabled[3];
	u8 raiseInterrupt[3];
	u32 value[3];          // value
	u32 direction[3];      // counting up or down
	u8* datetime_pointer;

	// what units will be? nanoseconds or cycles?
	u64 last_update;  // not exposed to emulated system, for emulation only

	void update_advance(u64 timing);

	// internal state

	void setRTC(rtc_state* newdatetime);
	void getRTC(rtc_state* datetime);
	void updateRTC(rtc_state* datetimeadvance);


	// memory mapped io
	memory* mem;
	u32 base_address;
	// port io
	ports* port;
	u32 port_index;

	// serialize and unserialize
	void save(u8* buf);
	void load(u8* buf);

};

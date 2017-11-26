
#include "timer.hpp"


timer::timer() {
	datetime_pointer = nullptr;
	last_update = 0;

	mem = nullptr;
	base_address = 0;
	port = nullptr;
	port_index = 0xCC;

	last_update = 0;
}

timer::~timer() {

}

void timer::update_advance(u64 timing) {
	UNUSED(timing);
	// unimplemented
}

void timer::setRTC(rtc_state* newdatetime) {
	UNUSED(newdatetime);
	// unimplemented
}

void timer::getRTC(rtc_state* newdatetime) {
	UNUSED(newdatetime);
	// unimplemented
}

void timer::updateRTC(rtc_state* datetimeadvance) {
	UNUSED(datetimeadvance);
	// unimplemented
}

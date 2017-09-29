
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
	// TODO implement
}

void timer::setRTC(rtc_state* newdatetime) {
	// TODO implement
}

void timer::getRTC(rtc_state* newdatetime) {
	// TODO implement
}

void timer::updateRTC(rtc_state* datetimeadvance) {
	// TODO implement
}

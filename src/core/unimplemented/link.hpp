
#pragma once

#include "../../common/common.hpp"

// serial, usb, cable, wireless

struct cable {
	bool transfering;
	u8* incoming_data;
	u8* outgoing_data;
	u32 incoming_index;
	u32 outgoing_index;
};

struct wireless {
	
};

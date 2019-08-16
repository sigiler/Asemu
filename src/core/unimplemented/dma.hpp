
#pragma once

#include "../../common/common_types.hpp"

struct dma {
	// direct memory access
	// basically hardware dedicated to copy data
	u32 cnt;  // control
	u32 src;  // source
	u32 dst;  // destination
	u32 len;  // length
};

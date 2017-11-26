
#pragma once

#include "../common/common_types.hpp"
#include "../common/common_macros.hpp"
#include <vector>

// constants

enum memory_type {
	M_SIMPLE,
	M_PERSISTENT,     // FLASH
	M_NONPERSISTANT,  // RAM
	M_PHYSICAL,
	M_VIRTUAL,        // virtual memory rules
	M_INTERNAL,       // state of IO devices
	M_VOID
};

enum memory_rights {
	M_READ = 4,
	M_WRITE = 2,
	M_EXECUTE = 1,
	M_NONE = 0,
	M_ALL = 7,

	M_000 = 0,
	M_00X = 0,
	M_0W0 = 0,
	M_0WX = 0,
	M_R00 = 0,
	M_R0X = 0,
	M_RW0 = 0,
	M_RWX = 7,

	M_PREVILIGED = 8,
};

// physical memory

struct memory {

	u8 type;
	u8 rights;
	u64 size;
	u8* buffer;

	memory();
	memory(u64 s);
	~memory();

	u8 read_byte(u32 a);
	void write_byte(u32, u8 a);

	u8 get_rights();
	void set_rights(u8 r);

	void save(std::vector<u8> buf);
	void load(std::vector<u8> buf);

};

// virtual memory

struct virtual_rule {
	memory* real_mem;
	u8 virtual_rights[16];
	u64 virtual_start;
	u64 virtual_end;
	u64 real_start;
	u64 real_end;
	u64 length;
};

struct memory_virtual {

	virtual_rule r[16];

	memory_virtual();
	~memory_virtual();

	u8 read_byte(u32 a);
	void write_byte(u32, u8 a);

	u8 get_rights();
	void set_rights(u8 r);

	void add_map(u32 address, u32 size);
	void rem_map(u32 address, u32 size);

	void save(std::vector<u8> buf);
	void load(std::vector<u8> buf);

};

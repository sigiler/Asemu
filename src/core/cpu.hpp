
#pragma once

#include <vector>
#include <string>
#include <cassert>

#include "../common/common_macros.hpp"
#include "../common/common_types.hpp"
#include "../common/type_u24.hpp"
#include "../common/type_b24.hpp"
#include "../common/type_v24.hpp"

#include "mem.hpp"
#include "ports.hpp"

struct registers {

	static const u8 number = 12;

	union {
		u24 r[12];

		RegView<0> a;
		RegView<1> b;
		RegView<2> c;
		RegView<3> x;
		RegView<4> y;
		RegView<5> z;

		union {
			RegFlagView<6> fs;  // flags and status register
			union {
				FlagView<6, 23> p;   // in privileged mode
				FlagView<6, 22> i;   // in interrupt mode
				FlagView<6, 21> e;   // in exception mode
				FlagView<6, 20> ie;  // interrupts enabled
				FlagView<6, 19> ee;  // exceptions enabled
				FlagView<6, 18> v;   // virtual memory enabled
			} s;
			union {
				FlagView<6, 3> v;  // overflow flag
				FlagView<6, 2> n;  // negative flag
				FlagView<6, 1> z;  // zero flag
				FlagView<6, 0> c;  // carry flag
			} f;
		};

		RegView<7> tp;   // trap pointer
		RegView<8> ip;  // interrupt pointer
		RegView<9> ep;  // exception pointer
		RegView<10> sp;   // stack pointer
		RegView<11> cp;   // code pointer (aka program counter)

		RegViewRange<0, 6> gpr;
		RegViewRange<6, 12> spr;
	};

	registers() : r() {}
};


using instruction = void (*) (struct cpu*);

struct cpu {

	registers regs;
	memory* mem;
	ports* prt;
	const instruction* instr_set;

	// properties
	u64 clock_rate;  // number of cycles executed per second

	// state
	u8 mode;
	u8 ackint;
	u32 opcode;
	u64 cyclesExecuted;  // number of executed cycles since restart or wrap
	//u64 cycleNextEvent;
	u64 cyclesUntilEvent;
	u64 cyclesPerFrame;
	//bool eventScheduled;

	// emulation state
	bool ended = false;
	bool running = false;

	// report unexpected
	void error(std::string description);

	// read write operations
	inline u8 read_byte(u32 a);
	inline void write_byte(u32 a, u8 v);
	inline u32 read_word(u32 a);
	inline void write_word(u32 a, u32 v);
	void push_word(u32 v);
	u32 pop_word();

	u8 in_byte(u32 p);
	void out_byte(u32 p, u32 v);
	u32 in_word(u32 p);
	void out_word(u32 p, u32 v);

	// fetching
	inline void pre_fetch_byte();
	inline u8 fetch_byte();
	inline u8 fetch_next_byte();
	inline u16 fetch_pair();
	inline u32 fetch_word();
	// decoding

	// addressing modes
	u32 read_reg_mode1_word(u8 r);
	void write_reg_mode1_word(u8 r, u32 v);
	u8 read_reg_mode1_byte(u8 r);
	void write_reg_mode1_byte(u8 r, u8 v);

	// flags check
	u8 conditions(u8 conditions);

	// interrupts
	void interrupt();
	void exception();

	// public part
	cpu();
	~cpu();

	void power();
	void reset();

	void step();
	u64 advance(u64);

	void run();
	void endrun();
	void resume();
	void pause();

	void save(std::vector<u8> buf);
	void load(std::vector<u8> buf);

};


// opcode mnemomics
enum OPCODE : u8 {
	NOP,
	LDR,
	LDB,
	LDS,
	ADD,
	SUB,
	MUL,
	DIV,
	REM,
	NEG,
	CMP,
	AND,
	OR,
	XOR,
	NOT,
	ROL,
	ROR,
	RCL,
	RCR,
	SHL,
	SHR,
	JMP,
	PUSH,
	POP,
	CALL,
	RET,
	IN,
	OUT,
	SPE1,
	SPE2,
};

// operand decode group/type
enum GROUP {
	R_Cw,       // LDR
	R_Cb,       // LDB
	RR_RR_w,    // LDR
	RR_RR_b,    // LDB
	RS_RS,      // LDS
	R_RR_RR,    // ADD, SUB, MUL, DIV, REM, rotations and shifts
	R_RR,       // NEG, NOT
	FF_C24,     // JMP, CALL FF,abs
	FF_C20,     // JMP, CALL FF,rel
	FF_C16,     // JMP, CALL FF,rel
	FF_C12,     // JMP, CALL FF,rel
	FF_C8,      // JMP, CALL FF,rel
	FF_REG,     // JMP, CALL reg
	FF,         // RET
	REGSET,     // PUSH POP
	RR_RR_port, // IN OUT

	IMPLICIT,
	NONE,
	NOOP
};


// registers and addressing modes

enum REG_MODE : u8 {
	REG_A = 0,
	REG_B = 1,
	REG_C = 2,
	REG_X = 3,
	REG_Y = 4,
	REG_Z = 5,
	REG_A_ind,
	REG_B_ind,
	REG_C_ind,
	REG_X_ind,
	REG_Y_ind,
	REG_Z_ind,
	REG_A_ind_inc,
	REG_A_ind_dec,
	REG_X_ind_inc,
	REG_X_ind_dec,
	REG_A_ind_inc_b,
	REG_A_ind_dec_b,
	REG_X_ind_inc_b,
	REG_X_ind_dec_b,
	REG_A_ind_b,
	REG_B_ind_b,
	REG_C_ind_b,
	REG_X_ind_b,
	REG_Y_ind_b,
	REG_Z_ind_b,
	REG_SP,
	REG_CP,
	REG_FS
};

enum REGINDEX : u8 {
	R_A,
	R_B,
	R_C,
	R_X,
	R_Y,
	R_Z,
	R_FS,
	R_TP,
	R_IP,
	R_EP,
	R_SP,
	R_CP,
};

enum REGSET : u8 {
	RS_A = 1 << 0,
	RS_B = 1 << 1,
	RS_C = 1 << 2,
	RS_X = 1 << 3,
	RS_Y = 1 << 4,
	RS_Z = 1 << 5,
	RS_FS = 1 << 6,
	RS_SP = 1 << 7
};

// flags and conditions

enum FLAGS : u32 {
	Z_MASK = 1 << 3,
	C_MASK = 1 << 2,
	V_MASK = 1 << 1,
	N_MASK = 1 << 0,
};

enum CONDS : u8 {
	always = 0x00,
	eq,
	ne,
	gt,
	lt,
	ge,
	le,
	a,
	b,
	ae,
	be,
	random_c,
	carry,
	noncarry,
	zero,
	nonzero,
	positive,
	negative,
	overflow,
	nonoverflow,
	never,
};
